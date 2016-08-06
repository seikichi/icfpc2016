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

mpq_class Area(const Polygon &p) {
  mpq_class ret = 0;
  for (int i = 0; i < (int)p.size(); i++) {
    ret += cross(CURR(p, i), NEXT(p, i));
  }
  return ret / 2.0;
}

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

// 点pをangleだけ回転する
// angleは回転する角度を表す直角三角形の各辺の長さ (例えば (5, 12, 13))
Point RotatePointByAngle(const Point& p, const std::tuple<int, int, int>& angle) {
  mpq_class c(std::get<0>(angle), std::get<2>(angle));
  mpq_class s(std::get<1>(angle), std::get<2>(angle));
  return Point(p.real() * c - p.imag() * s,
               p.real() * s + p.imag() * c);
}

// 点pをangleだけ逆回転する
// angleは回転する角度を表す直角三角形の各辺の長さ (例えば (5, 12, 13))
Point RotatePointByAngleReverse(const Point& p, const std::tuple<int, int, int>& angle) {
  mpq_class c(std::get<0>(angle), std::get<2>(angle));
  mpq_class s(std::get<1>(angle), std::get<2>(angle));
  return Point(p.real() * c + p.imag() * s,
              -p.real() * s + p.imag() * c);
}
