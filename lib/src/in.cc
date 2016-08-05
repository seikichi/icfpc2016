#include "in.h"

#include "inout.h"
#include <assert.h>

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
void Input::MakeSilhouettesD(const Point &offset) const {
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
