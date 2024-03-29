#pragma once

#include <vector>
#include <string>
#include <utility>
#include <complex>
#include <gmpxx.h>

using Point = std::complex<mpq_class>;
using Polygon = std::vector<Point>;
using Silhouette = std::vector<Polygon>;
using Segment = std::pair<Point, Point>;
using Skeleton = std::vector<Segment>;

// 与えられた文字列を分数として読む
mpq_class ReadFraction(std::string s);
// 与えられた座標を読む
Point ReadPoint(std::string s);

// 標準入力からsilhouetteを読み込む
Silhouette ReadSilhouette();
// 標準入力からskeletonを読み込む
Skeleton ReadSkeleton();
