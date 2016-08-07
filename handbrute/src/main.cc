#include "input.h"
#include "score.h"
#include "geometry.h"
#include "fold.h"
#include <iostream>
#include <tuple>
using namespace std;

Silhouette
RotateSihouette(const Silhouette& silhouette, const tuple<mpq_class, mpq_class, mpq_class>& angle) {

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

Silhouette
RotateSihouetteReverse(const Silhouette& silhouette, const tuple<mpq_class, mpq_class, mpq_class>& angle) {

  Silhouette rotated_silhouette;
  rotated_silhouette.reserve(silhouette.size());
  for (auto& polygon : silhouette) {
    Polygon rotated_polygon;
    rotated_polygon.reserve(polygon.size());
    for (auto& point : polygon) {
      rotated_polygon.push_back(RotatePointByAngleReverse(point, angle));
    }
    rotated_silhouette.push_back(move(rotated_polygon));
  }
  return rotated_silhouette;
}


vector<Line>
RotateSkeleton(const vector<Line>& skeleton, const tuple<mpq_class, mpq_class, mpq_class>& angle) {

  vector<Line> result;
  result.reserve(skeleton.size());
  for (auto& line : skeleton) {
    result.emplace_back(RotatePointByAngle(line[0], angle),
                        RotatePointByAngle(line[1], angle));
  }
  return result;
}

vector<Line>
RotateSkeletonReverse(const vector<Line>& skeleton, const tuple<mpq_class, mpq_class, mpq_class>& angle) {

  vector<Line> result;
  result.reserve(skeleton.size());
  for (auto& line : skeleton) {
    result.emplace_back(RotatePointByAngleReverse(line[0], angle),
                        RotatePointByAngleReverse(line[1], angle));
  }
  return result;
}

Silhouette
TranslateSihouette(const Silhouette& silhouette, const Point& offset) {

  Silhouette translated_silhouette;
  translated_silhouette.reserve(silhouette.size());
  for (auto& polygon : silhouette) {
    Polygon translated_polygon;
    translated_polygon.reserve(polygon.size());
    for (auto& point : polygon) {
      translated_polygon.push_back(point + offset);
    }
    translated_silhouette.push_back(move(translated_polygon));
  }
  return translated_silhouette;
}

vector<Line>
TranslateSkeleton(const vector<Line>& skeleton, const Point& offset) {
  vector<Line> result;
  result.reserve(skeleton.size());
  for (auto& line : skeleton) {
    result.emplace_back(line[0] + offset, line[1] + offset);
  }
  return result;
}

/*
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
*/

// TODO: 富豪的すぎるのでなんとかしたい
pair<double, Output>
Dfs(const Input& input, vector<Line> creases, vector<Line> candidates,
    int depth, int max_depth, const vector<Line>& convex, const Output& initial) {
  // calculate current score
  Output curr_output = initial;
  for (auto it = creases.rbegin(); it != creases.rend(); ++it) {
    curr_output = Fold(curr_output, *it);
  }
  double curr_score = ScoringMonte(input, curr_output, 1000, false);
  if (depth == max_depth) {
    return make_pair(curr_score, move(curr_output));
  }
  // choose a line from candidates and call Dfs
  double max_score = curr_score;
  Output best_output = move(curr_output);
  const vector<Line>& cands = (depth == 0) ? convex : candidates;
  for (auto line : cands) {
    for (int dir = 0; dir < 2; ++dir) {
      if (dir == 1) {
        line = Line(line[1], line[0]);
      }
      vector<Line> new_creases(creases);
      new_creases.push_back(line);
      vector<Line> new_candidates(candidates);
      for (auto& l : candidates) {
        if (l == line) continue;
        new_candidates.push_back(reflection(line, l));
      }
      double score;
      Output output;
      tie(score, output) = Dfs(input, new_creases, new_candidates, depth+1, max_depth,
                               convex, initial);
      if (score == 1.0) {       // early return
        return make_pair(score, move(output));
      }
      if (score > max_score) {
        max_score = score;
        best_output = move(output);
      }
    }
  }
  return make_pair(max_score, move(best_output));
}

vector<tuple<Point, int, tuple<mpq_class, mpq_class, mpq_class>>>
FindRightAngles(const vector<Line>& skeltons) {
  vector<tuple<Point, int, tuple<mpq_class, mpq_class, mpq_class>>> result;
  for (int i = 0; i < (int)skeltons.size(); ++i) {
    for (int j = 0; j < (int)skeltons.size(); ++j) {
      if (i == j) continue;
      const Line& line1 = skeltons[i];
      const Line& line2 = skeltons[j];
      for (int ii = 0; ii < 2; ++ii) {
        for (int jj = 0; jj < 2; ++jj) {
          if (line1[ii] == line2[jj]) {
            if (dot(line1[1-ii] - line1[ii], line2[1-jj] - line2[jj]) == 0) {
              mpq_class x = real(line1[1-ii]) - real(line1[ii]);
              mpq_class y = imag(line1[1-ii]) - imag(line1[ii]);
              mpq_class z2 = x*x + y*y;
              mpz_class n2 = z2.get_num() * z2.get_den();
              mpz_class n = sqrt(n2);
              if (n*n == n2) {
                Point p = line1[ii];
                tuple<mpq_class, mpq_class, mpq_class> angle(
                    x, y, mpq_class(n, z2.get_den()));
                result.emplace_back(p, ccw(line1[ii], line1[1-ii], line2[1-jj]), angle);
              }
            }
          }
        }
      }
    }
  }
  result.emplace_back(Point(0, 0), 0, make_tuple(1, 0, 1));
  return result;
}

pair<double, Output> Solve(const Input& input, const Output& initial, int max_depth) {
  if (input.skeltons.size() >= 20) {
    cerr << "Too many lines in skeletone. Give up!!" << endl;
    return make_pair(0, initial);
  }

  const vector<Line>& skeltons = input.skeltons;

  double max_score = -1;
  Output best_output;

  vector<tuple<Point, int, tuple<mpq_class, mpq_class, mpq_class>>> right_angles =
    FindRightAngles(skeltons);

  int limit = (max_depth <= 1 ? right_angles.size() : 1);
  for (int k = 0; k < limit; ++k) {
    auto& right_angle = right_angles[k];
    Point corner;
    int ccw_v;
    tuple<mpq_class, mpq_class, mpq_class> angle;
    tie(corner, ccw_v, angle) = right_angle;

    Silhouette silhouette = RotateSihouetteReverse(
        TranslateSihouette(input.silhouettes, -corner), angle);
    if (ccw_v == -1) {
      silhouette = TranslateSihouette(silhouette, Point(0, 1));
    }

    vector<Line> skeleton = RotateSkeletonReverse(
        TranslateSkeleton(input.skeltons, -corner), angle);
    if (ccw_v == -1) {
      skeleton = TranslateSkeleton(skeleton, Point(0, 1));
    }

    Input transformed_input;
    transformed_input.silhouettes = silhouette;
    transformed_input.skeltons = skeleton;

    // find the first polygon that have positive area
    auto it = find_if(silhouette.begin(), silhouette.end(),
        [](const Polygon& polygon) { return Area(polygon) > 0; });
    if (it == silhouette.end()) {
      cerr << "Could not find the polygon with positive area. Maybe BUG!\n";
      exit(1);
    }
    Polygon convex_hull = ConvexHull(*it);

    vector<Line> convex;
    for (int i = 0; i < (int)convex_hull.size(); ++i) {
      convex.emplace_back(convex_hull[i], convex_hull[(i+1)%convex_hull.size()]);
    }

    vector<Line> candidates;
    for (auto& line : skeleton) {
      bool use = true;
      for (auto& l : candidates) {
        if (SameLine(line, l)) {
          use = false;
          break;
        }
      }
      if (use) {
        candidates.push_back(line);
      }
    }

    double score;
    Output output;
    tie(score, output) = Dfs(transformed_input, vector<Line>(), candidates, 0, max_depth, convex, initial);

    if (ccw_v == -1) {
      output.dest_points = TranslatePolygon(output.dest_points, Point(0, -1));
    }
    output.dest_points = TranslatePolygon(RotatePolygon(output.dest_points, angle), corner);

    if (score > max_score) {
      max_score = score;
      best_output = move(output);
      if (score == 1.0) {
        return make_pair(max_score, best_output);
      }
    }
  }

  return make_pair(max_score, best_output);
}

int main(int argc, char** argv) {
  if (argc != 2 && argc != 3) {
    cerr << "Usage: handbrute INITIAL_SOLUTION [MAX_DEPTH] < PROBLEM\n";
    exit(1);
  }

  Output initial;
  initial.ReadOutput(argv[1]);

  int max_depth = 2;
  if (argc >= 3) {
    max_depth = stoi(argv[2]);
  }

  Input input;
  input.ReadInput(stdin);

  double max_score = -1;
  Output best_output;

  for (int i = 0; i < 4; ++i) {
    double score;
    Output output;
    tie(score, output) = Solve(input, initial, max_depth);
    if (score == 1.0) {
      best_output = move(output);
      break;
    }
    if (score > max_score) {
      max_score = score;
      best_output = move(output);
    }
    Point half(mpq_class(1, 2), mpq_class(1, 2));
    initial.dest_points = TranslatePolygon(
        RotatePolygon(TranslatePolygon(initial.dest_points, -half), make_tuple(0, 1, 1)),
        half);
  }

  best_output.WriteOutput(stdout);
}
