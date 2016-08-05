#pragma once

#include "geometry.h"
#include <gmpxx.h>
#include <vector>

struct Output {
  std::vector<Point> source_points;
  std::vector<std::vector<int>> facet_indecies;
  std::vector<Point> dest_points;
  bool ReadOutput(const char *filename);
  bool ReadOutput(FILE *file);
  std::vector<mpq_class> MinMaxXY() const;

  void MakeFacetD(const Point &offset) const;
  bool ContainFacet(const PointD &p) const;
  mutable std::vector<PolygonD> facet_polygons_d; // 向きを正規化して入れる
};
