#pragma once

#include "geometry.h"
#include <gmpxx.h>
#include <vector>

struct Input {
  std::vector<Polygon> silhouettes;
  std::vector<Line> skeltons;
  std::vector<PolygonD> silhouettes_d; // 向きを正規化して入れる
  std::vector<char> flip_flags;
  bool ReadInput(const char *filename);
  bool ReadInput(FILE *file);
  void MakeSilhouettesD(const Point &offset);
  bool ContainSilhouette(const PointD &p) const;
  std::vector<mpq_class> MinMaxXY() const;
};

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
