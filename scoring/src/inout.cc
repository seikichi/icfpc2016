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
mpq_class ReadFrac(FILE *file) {
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
  mpz_class num(a);
  mpz_class den(b);
  mpq_class q(num, den);
  return q;
}
Point ReadPoint(FILE *file) {
  mpq_class x = ReadFrac(file);
  int v = fscanf(file, " ,");
  assert(v == 0);
  mpq_class y = ReadFrac(file);
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
void Input::MakeSilhouettesD(const Point &offset) {
  int n = silhouettes.size();
  silhouettes_d.resize(n);
  flip_flags.resize(n);
  for (int i = 0; i < n; i++) {
    int m = silhouettes[i].size();
    silhouettes_d[i].resize(m);
    for (int j = 0; j < m; j++) {
      silhouettes_d[i][j] = mpq2d(silhouettes[i][j] - offset);
    }
    flip_flags[i] = Area(silhouettes_d[i]) < 0;
    if (flip_flags[i]) {
      reverse(silhouettes_d[i].begin(), silhouettes_d[i].end());
    }
  }
}
bool Input::ContainSilhouette(const PointD &p) const {
  bool ret = false;
  for (int i = 0; i < (int)silhouettes_d.size(); i++) {
    const auto &silhouette_d = silhouettes_d[i];
    bool contain = Contains(silhouette_d, p) == ContainResult::IN;
    if (flip_flags[i] && contain) { return false; }
    if (contain) { ret = true; }
  }
  return ret;
}
std::vector<mpq_class> Input::MinMaxXY() const {
  Point s = silhouettes[0][0];
  std::vector<mpq_class> ret = { s.real(), s.imag(), s.real(), s.imag() };
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
  return true;
}
void Output::MakeFacetD(const Point &offset) {
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
