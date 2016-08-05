#pragma once

#include <vector>
#include <string>
#include <utility>
#include <gmpxx.h>

struct Point {
  mpq_class x, y;
  Point(const mpq_class& x, const mpq_class& y) : x(x), y(y) {}
};
using Polygon = std::vector<Point>;
using Silhouette = std::vector<Polygon>;
using Segment = std::pair<Point, Point>;
using Skeleton = std::vector<Segment>;

mpq_class ReadFraction(std::string s);
Point ReadPoint(std::string s);
Silhouette ReadSilhouette();
Skeleton ReadSkeleton();

