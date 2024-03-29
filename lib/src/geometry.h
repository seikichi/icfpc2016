#pragma once

#include <gmpxx.h>
#include <complex>
#include <tuple>

typedef std::complex<mpq_class> Point;
typedef std::vector<Point> Polygon;
typedef std::complex<double> PointD;
typedef std::vector<PointD> PolygonD;

// static const double INF = 1e+10;

struct Line : public std::vector<Point> {
  Line() {;}
  Line(Point a, Point b) { push_back(a); push_back(b); }
};

namespace std {
  bool operator<(const Point &lhs, const Point &rhs);
}

template<class T>
void canonicalize(std::complex<T> &p) {
  T x = p.real();
  T y = p.imag();
  x.canonicalize(); y.canonicalize();
  p = Point(x, y);
}

PointD mpq2d(const Point &p);
PolygonD mpq2d(const Polygon &ps);

template<class T>
T cross(const std::complex<T> &a, const std::complex<T> &b) {
  return imag(conj(a) * b);
}

template<class T>
T dot(const std::complex<T> &a, const std::complex<T> &b) {
  return real(conj(a) * b);
}

template<class T>
int ccw(const std::complex<T> &a, std::complex<T> b, std::complex<T> c) {
  b -= a;
  c -= a;
  if (cross(b, c) > 0) { return 1; }
  if (cross(b, c) < 0) { return -1; }
  //if (dot(b, c) < 0) { return 2; }
  //if (norm(b) < norm(c)) { return -2; }
  return 0;
}

inline bool SameLine(const Line &l, const Line &s) {
  return cross(l[1] - l[0], s[1] - s[0]) == 0 &&
         cross(l[1] - l[0], s[0] - l[0]) == 0;
}

inline bool intersectLS(const Line &l, const Line &s) {
  return cross(l[1] - l[0], s[0] - l[0]) *
         cross(l[1] - l[0], s[1] - l[0]) <= 0;
}

inline Point projection(const Line &l, const Point &p) {
  mpq_class t = dot(p - l[0], l[0] - l[1]) / norm(l[0] - l[1]);
  Point q = l[0] + t * (l[0] - l[1]);
  canonicalize(q);
  return q;
}

inline Point reflection(const Line &l, const Point &p) {
  Point q = p + mpq_class(2,1) * (projection(l, p) - p);
  canonicalize(q);
  return q;
}

// sをlで反射する
inline Line reflection(const Line &l, const Line &s) {
  return Line(reflection(l, s[0]), reflection(l, s[1]));
}

inline Point crosspointLL(const Line &l, const Line &m) {
  mpq_class A = cross(l[1] - l[0], m[1] - m[0]);
  mpq_class B = cross(l[1] - l[0], l[1] - m[0]);
  mpq_class C = B / A;
  Point p = m[0] + C * (m[1] - m[0]);
  canonicalize(p);
  return p;
}

mpq_class Area(const Polygon &p);
double Area(const PolygonD &p);

#define CURR(P, i) (P[i])
#define NEXT(P, i) (P[(i + 1) % P.size()])
enum class ContainResult{ OUT, ON, IN };
template<class T>
ContainResult Contains(const std::vector<std::complex<T>>& P, const std::complex<T>& p) {
  bool in = false;
  for (int i = 0; i < (int)P.size(); ++i) {
    std::complex<T> a = CURR(P,i) - p, b = NEXT(P,i) - p;
    if (imag(a) > imag(b)) swap(a, b);
    if (imag(a) <= 0 && 0 < imag(b))
      if (cross(a, b) < 0) in = !in;
    if (cross(a, b) == 0 && dot(a, b) <= 0) return ContainResult::ON;
  }
  return in ? ContainResult::IN : ContainResult::OUT;
}
// ContainResult Contains(const Polygon &P, const PointF& p);

template<class T>
std::vector<std::complex<T>> ConvexHull(std::vector<std::complex<T>> ps) {
  int n = ps.size();
  int k = 0;
  sort(ps.begin(), ps.end());
  std::vector<std::complex<T>> ch(2 * n);
  for (int i = 0; i < n; ch[k++] = ps[i++])
    while (k >= 2 && ccw(ch[k - 2], ch[k - 1], ps[i]) <= 0) --k;
  for (int i = n - 2, t = k + 1; i >= 0; ch[k++] = ps[i--])
    while (k >= t && ccw(ch[k - 2], ch[k - 1], ps[i]) <= 0) --k;
  ch.resize(k - 1);
  return ch;
}

template<class T>
std::vector<std::complex<T>> ConvexCut(const std::vector<std::complex<T>> &P, const Line &l) {
  std::vector<std::complex<T>> Q;
  for (int i = 0; i < (int)P.size(); i++) {
    std::complex<T> A = CURR(P, i), B = NEXT(P, i);
    if (ccw(l[0], l[1], A) != -1) { Q.push_back(A); }
    if (ccw(l[0], l[1], A) * ccw(l[0], l[1], B) < 0) {
      Q.push_back(crosspointLL(Line(A, B), l));
    }
  }
  return Q;
}

// destinationにあるd3の点をsourceに射影する
// d3はd1-d2とs1-s2の直線上にある前提
template<class T>
std::complex<T> GetSourcePoint(
    const std::complex<T> d1, const std::complex<T> d2, const std::complex<T> d3,
    const std::complex<T> s1, const std::complex<T> s2) {
  T t = dot(d3 - d1, d1 - d2) / norm(d1 - d2);
  return s1 + t * (s1 - s2);
}

// 点pをangleだけ回転する
// angleは回転する角度を表す直角三角形の各辺の長さ (例えば (5, 12, 13))
Point RotatePointByAngle(const Point& p, const std::tuple<int, int, int>& angle);

// 点pをangleだけ逆回転する
// angleは回転する角度を表す直角三角形の各辺の長さ (例えば (5, 12, 13))
Point RotatePointByAngleReverse(const Point& p, const std::tuple<int, int, int>& angle);

// 点pをangleだけ回転する
// angleは回転する角度を表す直角三角形の各辺の長さ (例えば (5, 12, 13))
Point RotatePointByAngle(const Point& p, const std::tuple<mpq_class, mpq_class, mpq_class>& angle);

// 点pをangleだけ逆回転する
// angleは回転する角度を表す直角三角形の各辺の長さ (例えば (5, 12, 13))
Point RotatePointByAngleReverse(const Point& p, const std::tuple<mpq_class, mpq_class, mpq_class>& angle);

// Polygonをangleだけ回転させたPolygonを返す
template <typename N>
Polygon
RotatePolygon(const Polygon& polygon, const std::tuple<N, N, N>& angle) {
  Polygon rotated;
  rotated.reserve(polygon.size());
  for (auto& point : polygon) {
    rotated.push_back(RotatePointByAngle(point, angle));
  }
  return rotated;
}

// Polygonをangleだけ逆回転させたPolygonを返す
template <typename N>
Polygon
RotatePolygonReverse(const Polygon& polygon, const std::tuple<N, N, N>& angle) {
  Polygon rotated;
  rotated.reserve(polygon.size());
  for (auto& point : polygon) {
    rotated.push_back(RotatePointByAngleReverse(point, angle));
  }
  return rotated;
}

// Polygonをoffsetだけ平行移動させたPolygonを返す
Polygon TranslatePolygon(const Polygon& polygon, Point offset);
