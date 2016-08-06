#include "fold.h"
#include <map>

Output Fold(const Output &output, const Line &l) {
  std::map<Point, int> cut_point;
  Output ret;
  ret.source_points = output.source_points;
  ret.dest_points = output.dest_points;
  ret.facet_indecies = std::vector<std::vector<int>>();
  std::vector<int> ccws(output.dest_points.size());
  for (int i = 0; i < (int)output.dest_points.size(); i++){
    cut_point[output.source_points[i]] = i;
    ccws[i] = ccw(l[0], l[1], output.dest_points[i]);
    if (ccws[i]  == -1) {
      ret.dest_points[i] = reflection(l, output.dest_points[i]);
    }
  }

  for (int i = 0; i < (int)output.facet_indecies.size(); i++) {
    std::vector<int> q1_indecies;
    std::vector<int> q2_indecies;
    int q1_area_flag = 0;
    int q2_area_flag = 0;
    for (int j = 0; j < (int)output.facet_indecies[i].size(); j++) {
      int prev = output.facet_indecies[i][j];
      int next = output.facet_indecies[i][(j + 1) % output.facet_indecies[i].size()];
      const Point &A = output.dest_points[prev];
      const Point &B = output.dest_points[next];
      const int ccw_v = ccws[prev];
      const int ccw_w = ccws[next];
      if (ccw_v == 1) { q1_area_flag = 1; }
      if (ccw_v == -1) { q2_area_flag = 1; }
      if (ccw_v == 1 || ccw_v == 0) {
        q1_indecies.push_back(prev);
      }
      if (ccw_v == -1 || ccw_v == 0) {
        q2_indecies.push_back(prev);
      }
      if (ccw_v * ccw_w < 0) {
        const Point p = crosspointLL(Line(A, B), l);
        const Point q = GetSourcePoint(A, B, p, output.source_points[prev], output.source_points[next]);
        int index = ret.source_points.size();
        if (cut_point.count(q)) {
          index = cut_point[q];
        } else {
          ret.source_points.push_back(q);
          ret.dest_points.push_back(p);
          cut_point[q] = index;
        }
        q1_indecies.push_back(index);
        q2_indecies.push_back(index);
      }
    }
    if (q1_area_flag) {
      ret.facet_indecies.push_back(q1_indecies);
    }
    if (q2_area_flag) {
      reverse(q2_indecies.begin(), q2_indecies.end());
      ret.facet_indecies.push_back(q2_indecies);
    }
  }
  return ret;
}
