#include "out.h"

#include "inout.h"
#include <assert.h>

bool Output::ReadOutput(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == nullptr) {
    fprintf(stderr, "%s can't open!\n", filename);
    exit(1);
  }
  ReadOutput(file);
  fclose(file);
  return true;
}
bool Output::ReadOutput(FILE *file) {
  int point_num;
  int v = fscanf(file, " %d", &point_num);
  assert(v == 1);
  source_points.resize(point_num);
  dest_points.resize(point_num);
  for (int i = 0; i < point_num; i++) {
    Point p = ReadPoint(file);
    source_points[i] = p;
  }
  int facet_num;
  v = fscanf(file, " %d", &facet_num);
  assert(v == 1);
  facet_indecies.resize(facet_num);
  for (int i = 0; i < facet_num; i++) {
    int n;
    fscanf(file, " %d", &n);
    facet_indecies[i].resize(n);
    for (int j = 0; j < n; j++) {
      int index;
      fscanf(file, "%d", &index);
      facet_indecies[i][j] = index;
    }
  }
  for (int i = 0; i < point_num; i++) {
    Point p = ReadPoint(file);
    dest_points[i] = p;
  }
  return true;
}
bool Output::WriteOutput(const char *filename) const {
  FILE *file = fopen(filename, "w");
  if (file == nullptr) {
    fprintf(stderr, "%s can't open!\n", filename);
    exit(1);
  }
  WriteOutput(file);
  fclose(file);
  return true;
}
bool Output::WriteOutput(FILE *file) const {
  // source positions part
  fprintf(file, "%d\n", (int)source_points.size());
  for (auto &p : source_points) {
    fprintf(file, "%s,%s\n", p.real().get_str().c_str(), p.imag().get_str().c_str());
  }
  // facets part
  fprintf(file, "%d\n", (int)facet_indecies.size());
  for (auto &facets : facet_indecies) {
    fprintf(file, "%d", (int)facets.size());
    for (int index : facets) {
      fprintf(file, " %d", index);
    }
    fprintf(file, "\n");
  }
  // destination positions part
  for (auto &p : dest_points) {
    fprintf(file, "%s,%s\n", p.real().get_str().c_str(), p.imag().get_str().c_str());
  }
  return true;
}
void Output::MakeFacetD(const Point &offset) const {
  // make facet polygons
  facet_polygons_d.clear();
  for (auto indices : facet_indecies) {
    PolygonD facet;
    for (int index : indices) {
      facet.push_back(mpq2d(dest_points[index] - offset));
    }
    if (Area(facet) < 0) {
      reverse(facet.begin(), facet.end());
    }
    facet_polygons_d.push_back(facet);
  }
}
bool Output::ContainFacet(const PointD &p) const {
  for (const auto &facet : facet_polygons_d) {
    if (Contains(facet, p) == ContainResult::IN) { return true; }
  }
  return false;
}
std::vector<mpq_class> Output::MinMaxXY() const {
  Point s = dest_points[0];
  std::vector<mpq_class> ret = { s.real(), s.imag(), s.real(), s.imag() };
  for (const Point &p : dest_points) {
    ret[0] = std::min(ret[0], p.real());
    ret[1] = std::min(ret[1], p.imag());
    ret[2] = std::max(ret[2], p.real());
    ret[3] = std::max(ret[3], p.imag());
  }
  return ret;
}
