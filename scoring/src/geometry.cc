#include "geometry.h"

#include <algorithm>

namespace std {
  bool operator<(const Point &lhs, const Point &rhs) {
    return lhs.real() == rhs.real() ? lhs.imag() < rhs.imag() : lhs.real() < rhs.real();
  }
}

#define CURR(P, i) (P[i])
#define NEXT(P, i) (P[(i + 1) % P.size()])

mpf_class Area(const Polygon &p) {
  mpf_class ret = 0;
  for (int i = 0; i < (int)p.size(); i++) {
    ret += cross(CURR(p, i), NEXT(p, i));
  }
  return ret / 2.0;
}

ContainResult Contains(const Polygon& P, const Point& p) {
  bool in = false;
  for (int i = 0; i < (int)P.size(); ++i) {
    Point a = CURR(P,i) - p, b = NEXT(P,i) - p;
    if (imag(a) > imag(b)) swap(a, b);
    if (imag(a) <= 0 && 0 < imag(b))
      if (cross(a, b) < 0) in = !in;
    if (cross(a, b) == 0 && dot(a, b) <= 0) return ContainResult::ON;
  }
  return in ? ContainResult::IN : ContainResult::OUT;
}

Polygon ConvexHull(Polygon ps) {
  int n = ps.size();
  int k = 0;
  sort(ps.begin(), ps.end());
  std::vector<Point> ch(2 * n);
  for (int i = 0; i < n; ch[k++] = ps[i++])
    while (k >= 2 && ccw(ch[k - 2], ch[k - 1], ps[i]) <= 0) --k;
  for (int i = n - 2, t = k + 1; i >= 0; ch[k++] = ps[i--])
    while (k >= t && ccw(ch[k - 2], ch[k - 1], ps[i]) <= 0) --k;
  ch.resize(k - 1);
  return ch;
}
