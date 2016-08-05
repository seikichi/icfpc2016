#pragma once

#include <gmpxx.h>
#include <complex>

typedef std::complex<mpq_class> Point;
typedef std::vector<Point> Polygon;
typedef std::complex<double> PointD;
typedef std::vector<PointD> PolygonD;

// static const double INF = 1e+10;

struct Line : public std::vector<Point> {
  Line() {;}
  Line(Point a, Point b) { push_back(a); push_back(b); }
};

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
  if (dot(b, c) < 0) { return 2; }
  if (norm(b) < norm(c)) { return -2; }
  return 0;
}

// mpf_class Area(const Polygon &p);
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
  std::vector<Point> ch(2 * n);
  for (int i = 0; i < n; ch[k++] = ps[i++])
    while (k >= 2 && ccw(ch[k - 2], ch[k - 1], ps[i]) <= 0) --k;
  for (int i = n - 2, t = k + 1; i >= 0; ch[k++] = ps[i--])
    while (k >= t && ccw(ch[k - 2], ch[k - 1], ps[i]) <= 0) --k;
  ch.resize(k - 1);
  return ch;
}
