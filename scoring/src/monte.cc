#include "monte.h"
#include <gmpxx.h>

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

double Montecarlo(const char *input_filename, const char *solution_filename, int cnt) {
  Input input;
  Output output;
  input.ReadInput(input_filename);
  output.ReadOutput(solution_filename);
  std::vector<mpf_class> min_max_xy;
  {
    std::vector<mpq_class> min_max_xy1 = input.MinMaxXY();
    std::vector<mpq_class> min_max_xy2 = output.MinMaxXY();
    if (abs(min_max_xy1[0] - min_max_xy2[0]) > 10 || abs(min_max_xy1[1] - min_max_xy2[1]) > 10) { return 0.0; }
    min_max_xy1[0] = std::min(min_max_xy1[0], min_max_xy2[0]);
    min_max_xy1[1] = std::min(min_max_xy1[1], min_max_xy2[1]);
    min_max_xy1[2] = std::max(min_max_xy1[2], min_max_xy2[2]);
    min_max_xy1[3] = std::max(min_max_xy1[3], min_max_xy2[3]);
    input.MakeSilhouettesD(Point(min_max_xy1[0], min_max_xy1[1]));
    output.MakeFacetD(Point(min_max_xy1[0], min_max_xy1[1]));

    min_max_xy.push_back(mpq2mpf(min_max_xy1[0] - min_max_xy1[0]));
    min_max_xy.push_back(mpq2mpf(min_max_xy1[1] - min_max_xy1[1]));
    min_max_xy.push_back(mpq2mpf(min_max_xy1[2] - min_max_xy1[0]));
    min_max_xy.push_back(mpq2mpf(min_max_xy1[3] - min_max_xy1[1]));
  }

  gmp_randclass rnd(gmp_randinit_default);
  int area_and = 0;
  int area_or = 0;
  for (int i = 0; i < cnt; i++) {
    double x = rnd_next(&rnd, min_max_xy[0], min_max_xy[2]).get_d();
    double y = rnd_next(&rnd, min_max_xy[1], min_max_xy[3]).get_d();
    PointD p(x, y);
    bool contain_input = input.ContainSilhouette(p);
    bool contain_output = output.ContainFacet(p);
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
