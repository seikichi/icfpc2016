#pragma once

#include "geometry.h"
#include <gmpxx.h>
#include <vector>

struct Output {
  std::vector<Point> source_points;
  std::vector<std::vector<int>> facet_indecies;
  std::vector<Point> dest_points;
  std::vector<PolygonD> facet_polygons_d; // 向きを正規化して入れる
  bool ReadOutput(const char *filename);
  bool ReadOutput(FILE *file);
  void MakeFacetD(const Point &offset);
  bool ContainFacet(const PointD &p) const;
  std::vector<mpq_class> MinMaxXY() const;
};
