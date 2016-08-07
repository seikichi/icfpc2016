#pragma once

#include "geometry.h"
#include <gmpxx.h>
#include <string>
#include <vector>

struct Output {
  std::vector<Point> source_points;
  std::vector<std::vector<int>> facet_indecies;
  std::vector<Point> dest_points;
  void Init();
  bool ReadOutput(const char *filename);
  bool ReadOutput(FILE *file);
  bool WriteOutput(const char *filename) const;
  bool WriteOutput(FILE *file) const;
  std::string WriteString() const;
  bool Validate(bool verbose = false) const;

  bool WriteSVGSource(const char *filename) const;
  bool WriteSVGSource(FILE *file) const;
  bool WriteSVGDest(const char *filename) const;
  bool WriteSVGDest(FILE *file) const;
  std::vector<mpq_class> MinMaxXY() const;

  void MakeFacetD(const Point &offset) const;
  bool ContainFacet(const PointD &p) const;
  mutable std::vector<PolygonD> facet_polygons_d; // 向きを正規化して入れる
  mutable std::vector<std::vector<double>> facet_rects;
  mutable PolygonD facet_convex;
};
