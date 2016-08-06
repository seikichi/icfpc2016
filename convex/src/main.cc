#include "input.h"
#include "score.h"
#include "geometry.h"
#include "fold.h"
#include <iostream>
#include <tuple>
using namespace std;

vector<tuple<int, int, int>> angles = {
  make_tuple(1, 0, 1),  // 0.000000
  /*
  make_tuple(9940, 141, 9941),  // 0.812692
  make_tuple(1443, 76, 1445),  // 3.01487
  make_tuple(8265, 728, 8297),  // 5.03375
  make_tuple(84, 13, 85),  // 8.79741
  make_tuple(60, 11, 61),  // 10.3889
  make_tuple(40, 9, 41),  // 12.6804
  make_tuple(63, 16, 65),  // 14.25
  make_tuple(24, 7, 25),  // 16.2602
  make_tuple(35, 12, 37),  // 18.9246
  */
  make_tuple(12, 5, 13),  // 22.6199
  /*
  make_tuple(77, 36, 85),  // 25.0576
  make_tuple(15, 8, 17),  // 28.0725
  make_tuple(56, 33, 65),  // 30.5102
  make_tuple(91, 60, 109),  // 33.3985
  make_tuple(4, 3, 5),  // 36.8699
  make_tuple(55, 48, 73),  // 41.1121
  make_tuple(21, 20, 29),  // 43.6028
  */
  make_tuple(20, 21, 29),  // 46.3972
  /*
  make_tuple(65, 72, 97),  // 47.925
  make_tuple(48, 55, 73),  // 48.8879
  make_tuple(3, 4, 5),  // 53.1301
  make_tuple(60, 91, 109),  // 56.6015
  make_tuple(28, 45, 53),  // 58.1092
  make_tuple(8, 15, 17),  // 61.9275
  make_tuple(39, 80, 89),  // 64.0108
  make_tuple(5, 12, 13),  // 67.3801
  make_tuple(12, 35, 37),  // 71.0754
  make_tuple(7, 24, 25),  // 73.7398
  make_tuple(16, 63, 65),  // 75.75
  make_tuple(9, 40, 41),  // 77.3196
  make_tuple(20, 99, 101),  // 78.5788
  make_tuple(13, 84, 85),  // 81.2026
  make_tuple(820, 6699, 6749),  // 83.0214
  make_tuple(516, 7387, 7405),  // 86.0042
  make_tuple(141, 9940, 9941),  // 89.1873
  */
  make_tuple(-1, 0, 1),  // 180
};

Silhouette
RotateSihouette(const Silhouette& silhouette, const tuple<int, int, int>& angle) {

  Silhouette rotated_silhouette;
  rotated_silhouette.reserve(silhouette.size());
  for (auto& polygon : silhouette) {
    Polygon rotated_polygon;
    rotated_polygon.reserve(polygon.size());
    for (auto& point : polygon) {
      rotated_polygon.push_back(RotatePointByAngle(point, angle));
    }
    rotated_silhouette.push_back(move(rotated_polygon));
  }
  return rotated_silhouette;
}

tuple<mpq_class, mpq_class, mpq_class, mpq_class>
CalculateBoundsOfPolygon(const Polygon& polygon) {
  mpq_class min_x = polygon[0].real();
  mpq_class max_x = polygon[0].real();
  mpq_class min_y = polygon[0].imag();
  mpq_class max_y = polygon[0].imag();
  for (auto& point : polygon) {
    if (point.real() < min_x) min_x = point.real();
    if (point.real() > max_x) max_x = point.real();
    if (point.imag() < min_y) min_y = point.imag();
    if (point.imag() > max_y) max_y = point.imag();
  }
  return make_tuple(min_x, max_x, min_y, max_y);
}

tuple<double, int, Output>
CreateSolutionAndEvaluate(const Silhouette& silhouette, const tuple<int, int, int>& angle) {

  Silhouette rotated_silhouette = RotateSihouette(silhouette, angle);

  // find the first polygon that have positive area
  auto it = find_if(rotated_silhouette.begin(), rotated_silhouette.end(),
      [](const Polygon& polygon) { return Area(polygon) > 0; });
  if (it == rotated_silhouette.end()) {
    cerr << "Could not find the polygon with positive area. Maybe BUG!\n";
    exit(1);
  }
  Polygon original_convex_hull = ConvexHull(*it);

  mpq_class min_x, max_x, min_y, max_y;
  tie(min_x, max_x, min_y, max_y) = CalculateBoundsOfPolygon(original_convex_hull);

  Input input;
  input.silhouettes = silhouette;

  Output output;
  output.Init();

  Polygon convex_hull = TranslatePolygon(original_convex_hull, Point(-min_x, -min_y));
  for (int i = 0, last_updated = -1; ; i = (i+1) % convex_hull.size()) {
    const Point& curr = convex_hull[i];
    const Point& next = convex_hull[(i+1)%convex_hull.size()];
    size_t n_source_points = output.source_points.size();
    Output next_output = Fold(output, Line(curr, next));
    if ((int)next_output.WriteString().size() > 5000) { break; }
    output = next_output;
    if (output.source_points.size() > n_source_points) {
      last_updated = i;
    } else {
      if ((i+1) % convex_hull.size() == last_updated)
        break;
    }
  }

  output.dest_points = TranslatePolygon(output.dest_points, Point(min_x, min_y));
  output.dest_points = RotatePolygonReverse(output.dest_points, angle);

  double score = ScoringMonte(input, output, 1000);
  int n_bytes = output.WriteString().size();
  return make_tuple(score, n_bytes, move(output));
}

int main() {
  Silhouette silhouette = ReadSilhouette();
  Skeleton skeleton = ReadSkeleton();

  double max_score = -1;
  int min_n_bytes = -1;
  Output best_output;
  best_output.Init();

  for (auto& angle : angles) {
    double score;
    int n_bytes;
    Output output;
    tie(score, n_bytes, output) = CreateSolutionAndEvaluate(silhouette, angle );
    if (n_bytes > 5000)
      continue;
    if ((score > max_score) || (score == max_score && n_bytes < min_n_bytes)) {
      max_score = score;
      min_n_bytes = n_bytes;
      best_output = move(output);
    }
  }

  if (max_score == -1) {
    cerr << "No valid solutions found. Output dummy solution.\n";
  }
  best_output.WriteOutput(stdout);
}
