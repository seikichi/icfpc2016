#include "score.h"

#include "xor_shift.h"
#include <math.h>
#include <gmpxx.h>
#include <iostream>

mpf_class mpq2mpf(const mpq_class &q) {
  mpf_class v(q.get_num(), 256);
  return v / q.get_den();
}

mpf_class rnd_next(gmp_randclass *rnd, const mpf_class &r) {
  return rnd->get_f(256) * r;
}
mpf_class rnd_next(gmp_randclass *rnd, const mpf_class &l, const mpf_class &r) {
  return rnd_next(rnd, r - l) + l;
}

double ScoringMonte(const char *input_filename, const char *solution_filename, int cnt, bool full) {
  Input input;
  Output output;
  input.ReadInput(input_filename);
  output.ReadOutput(solution_filename);
  return ScoringMonte(input, output, cnt, full);
}

double ScoringMonte(const Input &input, const Output &output, int cnt, bool full) {
  if (!output.Validate()) { return 0.0; }
  std::vector<double> min_max_xy;
  {
    std::vector<mpq_class> min_max_xy1 = input.MinMaxXY();
    std::vector<mpq_class> min_max_xy2 = output.MinMaxXY();
    if (abs(min_max_xy1[0] - min_max_xy2[0]) > 5 || abs(min_max_xy1[1] - min_max_xy2[1]) > 5) { return 0.0; }
    min_max_xy1[0] = std::min(min_max_xy1[0], min_max_xy2[0]);
    min_max_xy1[1] = std::min(min_max_xy1[1], min_max_xy2[1]);
    min_max_xy1[2] = std::max(min_max_xy1[2], min_max_xy2[2]);
    min_max_xy1[3] = std::max(min_max_xy1[3], min_max_xy2[3]);
    input.MakeSilhouettesD(Point(min_max_xy1[0], min_max_xy1[1]));
    output.MakeFacetD(Point(min_max_xy1[0], min_max_xy1[1]));

    min_max_xy.push_back(mpq2mpf(min_max_xy1[0] - min_max_xy1[0]).get_d());
    min_max_xy.push_back(mpq2mpf(min_max_xy1[1] - min_max_xy1[1]).get_d());
    min_max_xy.push_back(mpq2mpf(min_max_xy1[2] - min_max_xy1[0]).get_d());
    min_max_xy.push_back(mpq2mpf(min_max_xy1[3] - min_max_xy1[1]).get_d());
  }

  Random rnd(12356789);
  int area_and = 0;
  int area_or = 0;
  for (int i = 0; i < cnt; i++) {
    double x = rnd.next(min_max_xy[0], min_max_xy[2]);
    double y = rnd.next(min_max_xy[1], min_max_xy[3]);
    PointD p(x, y);
    bool contain_input = input.ContainSilhouette(p);
    bool contain_output = output.ContainFacet(p);
    if (contain_input && contain_output) {
      area_and++;
    }
    if (contain_input || contain_output) {
      area_or++;
    }
    if (!full && area_and != area_or) { break; }
  }
  // std::cout << area_and << " " << area_or << " " << std::endl;
  return (double)area_and / area_or;
}

// bool PerfectHit(const char *input_filename, const char *solution_filename, int cnt) {
//   Input input;
//   Output output;
//   input.ReadInput(input_filename);
//   output.ReadOutput(solution_filename);
//   return PerfectHit(input, output, cnt);
// }
//
// bool PerfectHit(const Input &input, const Output &output, int cnt) {
//   std::vector<Point> input_points;
//   for (const auto &silhouette : input.silhouettes) {
//     for (const auto &p : silhouette) {
//       input_points.push_back(p);
//     }
//   }
//   sort(input_points.begin(), input_points.end());
//   std::vector<Point> output_points = output.dest_points;
//   sort(output_points.begin(), output_points.end());
//   int out_pos = 0;
//   for (int in_pos = 0; in_pos <(int)input_points.size(); in_pos++) {
//     while (out_pos < (int)output_points.size() && output_points[out_pos] < input_points[in_pos]) { out_pos++; }
//     if (out_pos == (int)output_points.size() || output_points[out_pos] != input_points[in_pos]) { return false; }
//   }
//   if (input.silhouettes.size() == 1) {
//     double score = ScoringMonte(input, output, cnt);
//     if (score != 1.0 && !std::isnan(score)) { return false; }
//   }
//   return true;
// }
