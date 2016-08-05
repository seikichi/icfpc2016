#pragma once

#include <gmpxx.h>
#include <complex>

typedef std::complex<mpf_class> Point;
typedef std::vector<Point> Polygon;

// static const double INF = 1e+10;

struct Line : public std::vector<Point> {
  Line() {;}
  Line(Point a, Point b) { push_back(a); push_back(b); }
};

inline mpf_class cross(const Point &a, const Point &b) {
  return imag(conj(a) * b);
}

inline mpf_class dot(const Point &a, const Point &b) {
  return real(conj(a) * b);
}

inline int ccw(Point a, Point b, Point c) {
  b -= a;
  c -= a;
  if (cross(b, c) > 0) { return 1; }
  if (cross(b, c) < 0) { return -1; }
  if (dot(b, c) < 0) { return 2; }
  if (norm(b) < norm(c)) { return -2; }
  return 0;
}

mpz_class Area(const Polygon &p);

enum class ContainResult{ OUT, ON, IN };
ContainResult Contains(const Polygon& P, const Point& p);
