#pragma once

#include "geometry.h"
#include <gmpxx.h>
#include <string>
#include <vector>

struct Input {
  std::vector<Polygon> silhouettes;
  std::vector<Line> skeltons;
  bool ReadInput(const char *filename);
  bool ReadInput(FILE *file);
  bool WriteInput(const char *filename) const;
  bool WriteInput(FILE *file) const;
  std::string WriteString() const;
  static bool Validate(const std::string &str);
  std::vector<mpq_class> MinMaxXY() const;

  void MakeSilhouettesD(const Point &offset) const;
  bool ContainSilhouette(const PointD &p) const;
  mutable std::vector<PolygonD> silhouettes_d; // 向きを正規化して入れる
  mutable std::vector<std::vector<double>> silhouettes_rects;
  mutable std::vector<char> flip_flags;
};
