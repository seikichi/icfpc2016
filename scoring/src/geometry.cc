#include "geometry.h"

#include <algorithm>

namespace std {
  bool operator<(const Point &lhs, const Point &rhs) {
    return lhs.real() == rhs.real() ? lhs.imag() < rhs.imag() : lhs.real() < rhs.real();
  }
}

PointD mpq2d(const Point &p) {
  return PointD(p.real().get_d(), p.imag().get_d());
}
PolygonD mpq2d(const Polygon &ps) {
  PolygonD ret;
  for (const auto &p : ps) { ret.push_back(mpq2d(p)); }
  return ret;
}

// mpq_class Area(const Polygon &p) {
//   mpf_class ret = 0;
//   for (int i = 0; i < (int)p.size(); i++) {
//     ret += cross(CURR(p, i), NEXT(p, i));
//   }
//   return ret / 2.0;
// }

double Area(const PolygonD &p) {
  double ret = 0;
  for (int i = 0; i < (int)p.size(); i++) {
    ret += cross(CURR(p, i), NEXT(p, i));
  }
  return ret / 2.0;
}

// ContainResult Contains(Polygon &P, const PointF& p) {
//   return Contains(mpq2mpf(P), p);
// }
