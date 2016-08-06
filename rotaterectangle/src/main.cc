#include "input.h"
#include "score.h"
#include "crease.h"
#include <iostream>
#include <tuple>
using namespace std;

vector<tuple<int, int, int>> angles = {
  make_tuple(1, 0, 1),  // 0.000000
  make_tuple(9940, 141, 9941),  // 0.812692
  make_tuple(1443, 76, 1445),  // 3.01487
  make_tuple(8265, 728, 8297),  // 5.03375
  make_tuple(84, 13, 85),  // 8.79741
  make_tuple(60, 11, 61),  // 10.3889
  make_tuple(40, 9, 41),  // 12.6804
  make_tuple(63, 16, 65),  // 14.25
  make_tuple(24, 7, 25),  // 16.2602
  make_tuple(35, 12, 37),  // 18.9246
  make_tuple(12, 5, 13),  // 22.6199
  make_tuple(77, 36, 85),  // 25.0576
  make_tuple(15, 8, 17),  // 28.0725
  make_tuple(56, 33, 65),  // 30.5102
  make_tuple(91, 60, 109),  // 33.3985
  make_tuple(4, 3, 5),  // 36.8699
  make_tuple(55, 48, 73),  // 41.1121
  make_tuple(21, 20, 29),  // 43.6028
  make_tuple(20, 21, 29),  // 46.3972
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
};

vector<mpq_class> scales = {mpq_class(1, 1), mpq_class(3, 4), mpq_class(1, 2), mpq_class(1, 4)};

Point RotatePointByAngle(Point p, tuple<int, int, int> angle) {
  mpq_class c(get<0>(angle), get<2>(angle));
  mpq_class s(get<1>(angle), get<2>(angle));
  return Point(p.real() * c - p.imag() * s,
               p.real() * s + p.imag() * c);
}

Point RotatePointByAngleReverse(Point p, tuple<int, int, int> angle) {
  mpq_class c(get<0>(angle), get<2>(angle));
  mpq_class s(get<1>(angle), get<2>(angle));
  return Point(p.real() * c + p.imag() * s,
              -p.real() * s + p.imag() * c);
}

string FractionToString(mpq_class f) {
  f.canonicalize();
  if (f.get_den() == 1)
    return f.get_num().get_str();
  else
    return f.get_num().get_str() + "/" + f.get_den().get_str();
}

string PointToString(Point p) {
  return FractionToString(p.real()) + "," + FractionToString(p.imag());
}

pair<double, Output> CreateSolutionAndEvaluate(
    const Silhouette& silhouette, const Skeleton&,
    tuple<int, int, int> angle, mpq_class scale_x, mpq_class scale_y) {

  Silhouette rotated_silhouette;
  for (auto& polygon : silhouette) {
    Polygon rotated_polygon;
    for (auto& point : polygon) {
      rotated_polygon.push_back(RotatePointByAngle(point, angle));
    }
    rotated_silhouette.push_back(move(rotated_polygon));
  }

  mpq_class xg(0, 1);
  mpq_class yg(0, 1);
  int count = 0;
  for (auto& polygon : rotated_silhouette) {
    for (auto& point : polygon) {
      xg += point.real();
      yg += point.imag();
      count++;
    }
  }
  xg /= count;
  yg /= count;

  mpq_class min_x = rotated_silhouette[0][0].real();
  mpq_class max_x = rotated_silhouette[0][0].real();
  mpq_class min_y = rotated_silhouette[0][0].imag();
  mpq_class max_y = rotated_silhouette[0][0].imag();

  for (auto& polygon : rotated_silhouette) {
    for (auto& point : polygon) {
      if (point.real() < min_x) min_x = point.real();
      if (point.real() > max_x) max_x = point.real();
      if (point.imag() < min_y) min_y = point.imag();
      if (point.imag() > max_y) max_y = point.imag();
    }
  }

  mpq_class width = (max_x - min_x) * scale_x;
  width.canonicalize();
  mpq_class height = (max_y - min_y) * scale_y;
  height.canonicalize();

  vector<pair<mpq_class, mpq_class>> x_creases = CalculateCreases(1, width);
  vector<pair<mpq_class, mpq_class>> y_creases = CalculateCreases(1, height);

  Input input;
  input.silhouettes = silhouette;
  Output output;

  // source positions part
  for (auto& y : y_creases) {
    for (auto& x : x_creases) {
      Point p = Point(x.first, y.first);
      output.source_points.push_back(p);
    }
  }
  // facets part
  for (int i = 0; i < (int)y_creases.size()-1; ++i) {
    for (int j = 0; j < (int)x_creases.size()-1; ++j) {
      vector<int> indices = {
        i*(int)x_creases.size() + j,
        i*(int)x_creases.size() + j+1,
        (i+1)*(int)x_creases.size() + j+1,
        (i+1)*(int)x_creases.size() + j,
      };
      output.facet_indecies.push_back(move(indices));
    }
  }
  // destination positions part
  mpq_class offset_x = xg - min(width, mpq_class(1)) / 2;
  mpq_class offset_y = yg - min(height, mpq_class(1)) / 2;
  for (auto& y : y_creases) {
    for (auto& x : x_creases) {
      Point p = Point(x.second + offset_x, y.second + offset_y);
      Point rp = RotatePointByAngleReverse(p, angle);
      output.dest_points.push_back(move(rp));
    }
  }

  // size check
  string outstr = output.WriteString();
  if (outstr.size() > 5000) {
    // output exceeds the size limit.
    // fallback to less accurate offset.
    double offset_xd = offset_x.get_d();
    double offset_yd = offset_y.get_d();
    mpq_class offset_x2;
    if (abs(offset_xd) < 1e14)
        offset_x2 = offset_xd;
    else
        offset_x2 = offset_x.get_num()/offset_x.get_den();
    mpq_class offset_y2;
    if (abs(offset_yd) < 1e14)
        offset_y2 = offset_yd;
    else
        offset_y2 = offset_y.get_num()/offset_y.get_den();
    output.dest_points.clear();
    for (auto& y : y_creases) {
      for (auto& x : x_creases) {
        Point p = Point(x.second + offset_x2, y.second + offset_y2);
        Point rp = RotatePointByAngleReverse(p, angle);
        output.dest_points.push_back(move(rp));
      }
    }
  }

  double score = ScoringMonte(input, output, 1000);
  return make_pair(score, move(output));
}

int main() {
  Silhouette silhouette = ReadSilhouette();
  Skeleton skeleton = ReadSkeleton();

  double max_score = -1;
  Output output;
  for (auto& angle : angles) {
    for (auto& scale_x : scales) {
      for (auto& scale_y : scales) {
        auto p = CreateSolutionAndEvaluate(silhouette, skeleton, angle, scale_x, scale_y);
        if (p.first > max_score) {
          max_score = p.first;
          output = move(p.second);
        }
      }
    }
  }

  output.WriteOutput(stdout);
}
