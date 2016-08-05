#pragma once

#include "geometry.h"
#include <gmpxx.h>
#include <vector>

struct Input {
  std::vector<Polygon> silhouettes;
  std::vector<Line> skeltons;
  bool ReadInput(const char *filename);
  bool ReadInput(FILE *file);
  std::vector<mpq_class> MinMaxXY() const;

  void MakeSilhouettesD(const Point &offset) const;
  bool ContainSilhouette(const PointD &p) const;
  mutable std::vector<PolygonD> silhouettes_d; // 向きを正規化して入れる
  mutable std::vector<char> flip_flags;
};
