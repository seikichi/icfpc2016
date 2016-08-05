#pragma once

#include "geometry.h"
#include <vector>

struct Input {
  std::vector<Polygon> silhouettes;
  std::vector<Line> skeltons;
  bool ReadInput(const char *filename);
  bool ReadInput(FILE *file);
  bool ConatainSilhouette(const Point &p) const;
  std::vector<mpf_class> MinMaxXY() const;
};

struct Output {
  std::vector<Point> source_points;
  std::vector<std::vector<int>> facet_indecies;
  std::vector<Point> dest_points;
  std::vector<Polygon> facet_polygons;
  bool ReadOutput(const char *filename);
  bool ReadOutput(FILE *file);
  bool ConatainFacet(const Point &p) const;
  std::vector<mpf_class> MinMaxXY() const;
};
