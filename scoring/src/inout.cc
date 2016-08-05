#include "inout.h"

#include <assert.h>
#include <gmpxx.h>

std::string GetDigits(FILE *file) {
  std::string ret;
  fscanf(file, " ");
  while (true) {
    int c = getc(file);
    assert(c >= 0);
    if (!isdigit(c) && c != '-') {
      ungetc(c, file);
      break;
    }
    ret += c;
  }
  return ret;
}
mpf_class ReadFrac(FILE *file) {
  std::string a = GetDigits(file);
  std::string b = "1";
  assert(a.size() > 0);
  int c = getc(file);
  assert(c >= 0);
  if (c != '/') {
    ungetc(c, file);
  } else {
    b = GetDigits(file);
    assert(b.size() > 0);
  }
  mpf_class f1(a, 256);
  mpf_class f2(b, 256);
  return f1 / f2;
}
Point ReadPoint(FILE *file) {
  mpf_class x = ReadFrac(file);
  int v = fscanf(file, " ,");
  assert(v == 0);
  mpf_class y = ReadFrac(file);
  return Point(x, y);
}

bool Input::ReadInput(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == nullptr) {
    fprintf(stderr, "%s can't open!\n", filename);
    exit(1);
  }
  ReadInput(file);
  fclose(file);
  return true;
}
bool Input::ReadInput(FILE *file) {
  int silhouette_num;
  int v = fscanf(file, " %d", &silhouette_num);
  assert(v == 1);
  silhouettes.resize(silhouette_num);
  for (int i = 0; i < silhouette_num; i++) {
    int n;
    v = fscanf(file, " %d", &n);
    assert(v == 1);
    silhouettes[i].resize(n);
    for (int j = 0; j < n; j++) {
      Point p = ReadPoint(file);
      silhouettes[i][j] = p;
    }
  }
  int skelton_num;
  v = fscanf(file, " %d", &skelton_num);
  assert(v == 1);
  skeltons.resize(skelton_num);
  for (int i = 0; i < skelton_num; i++) {
    Point p1 = ReadPoint(file);
    Point p2 = ReadPoint(file);
    skeltons[i] = Line(p1, p2);
  }
  return true;
}
bool Input::ConatainSilhouette(const Point &p) const {
  bool ret = false;
  for (Polygon silhouette : silhouettes) {
    if (Area(silhouette) < 0) {
      // 反転している箇所に含まれる場合は常に穴なので即返す
      reverse(silhouette.begin(), silhouette.end());
      if (Contains(silhouette, p) == ContainResult::IN) { return false; }
    } else {
      if (Contains(silhouette, p) == ContainResult::IN) { ret = true; }
    }
  }
  return ret;
}
std::vector<mpf_class> Input::MinMaxXY() const {
  std::vector<mpf_class> ret = { 1e+10, 1e+10, -1e+10, -1e+10 };
  for (const Polygon &silhouette : silhouettes) {
    for (const Point &p : silhouette) {
      ret[0] = std::min(ret[0], p.real());
      ret[1] = std::min(ret[1], p.imag());
      ret[2] = std::max(ret[2], p.real());
      ret[3] = std::max(ret[3], p.imag());
    }
  }
  return ret;
}

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

  // make facet polygons
  facet_polygons.clear();
  for (auto indices : facet_indecies) {
    Polygon poly;
    for (int index : indices) {
      poly.push_back(dest_points[index]);
    }
    facet_polygons.push_back(poly);
  }
  return true;
}
bool Output::ConatainFacet(const Point &p) const {
  for (Polygon facet : facet_polygons) {
    if (Area(facet) < 0) {
      reverse(facet.begin(), facet.end());
    }
    if (Contains(facet, p) == ContainResult::IN) { return true; }
  }
  return false;
}
std::vector<mpf_class> Output::MinMaxXY() const {
  std::vector<mpf_class> ret = { 1e+10, 1e+10, -1e+10, -1e+10 };
  for (const Polygon &facet : facet_polygons) {
    for (const Point &p : facet) {
      ret[0] = std::min(ret[0], p.real());
      ret[1] = std::min(ret[1], p.imag());
      ret[2] = std::max(ret[2], p.real());
      ret[3] = std::max(ret[3], p.imag());
    }
  }
  return ret;
}
