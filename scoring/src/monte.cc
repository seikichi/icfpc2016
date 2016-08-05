#include "monte.h"

#include <gmpxx.h>

mpf_class rnd_next(gmp_randclass *rnd, const mpf_class &r) {
  return rnd->get_f(256) * r;
}
mpf_class rnd_next(gmp_randclass *rnd, const mpf_class &l, const mpf_class &r) {
  return rnd_next(rnd, r - l) + l;
}

double Montecarlo(const char *input_filename, const char *solution_filename, int cnt) {
  Input input;
  Output output;
  input.ReadInput(input_filename);
  output.ReadOutput(solution_filename);
  std::vector<mpf_class> min_max_xy;
  {
    std::vector<mpf_class> min_max_xy1 = input.MinMaxXY();
    std::vector<mpf_class> min_max_xy2 = output.MinMaxXY();
    min_max_xy.push_back(std::min(min_max_xy1[0], min_max_xy2[0]));
    min_max_xy.push_back(std::min(min_max_xy1[1], min_max_xy2[1]));
    min_max_xy.push_back(std::max(min_max_xy1[2], min_max_xy2[2]));
    min_max_xy.push_back(std::max(min_max_xy1[3], min_max_xy2[3]));
  }

  gmp_randclass rnd(gmp_randinit_default);
  int area_and = 0;
  int area_or = 0;
  for (int i = 0; i < cnt; i++) {
    mpf_class x = rnd_next(&rnd, min_max_xy[0], min_max_xy[2]);
    mpf_class y = rnd_next(&rnd, min_max_xy[1], min_max_xy[3]);
    Point p(x, y);
    bool contain_input = input.ConatainSilhouette(p);
    bool contain_output = output.ConatainFacet(p);
    if (contain_input && contain_output) {
      area_and++;
    }
    if (contain_input || contain_output) {
      area_or++;
    }
  }
  // std::cout << area_and << " " << area_or << " " << std::endl;
  return (double)area_and / area_or;
}
