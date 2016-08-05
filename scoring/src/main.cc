#include <assert.h>

#include <algorithm>
#include <complex>
#include <iostream>
#include <memory>
#include <vector>

static int MONTE_COUNT = 10000;

typedef std::complex<double> Point;
typedef std::vector<Point> Polygon;

// static const double INF = 1e+10;

#define CURR(P, i) (P[i])
#define NEXT(P, i) (P[(i + 1) % P.size()])

struct Line : public std::vector<Point> {
  Line() {;}
  Line(Point a, Point b) { push_back(a); push_back(b); }
};

namespace std {
  bool operator<(const Point &lhs, const Point &rhs) {
    return lhs.real() == rhs.real() ? lhs.imag() < rhs.imag() : lhs.real() < rhs.real();
  }
}

inline double cross(const Point &a, const Point &b) {
  return imag(conj(a) * b);
}

inline double dot(const Point &a, const Point &b) {
  return real(conj(a) * b);
}

inline int ccw(Point a, Point b, Point c) {
  b -= a;
  c -= a;
  if (cross(b, c) > 0) { return 1; }
  if (cross(b, c) < 0) { return -1; }
  if (dot(b, c) < 0) { return 2; }
  if (norm(b) < norm(c)) { return -2; }
  return 0;
}

double Area(const Polygon &p) {
  double ret = 0;
  for (int i = 0; i < (int)p.size(); i++) {
    ret += cross(CURR(p, i), NEXT(p, i));
  }
  return ret / 2.0;
}

enum { OUT, ON, IN };
int Contains(const Polygon& P, const Point& p) {
  bool in = false;
  for (int i = 0; i < (int)P.size(); ++i) {
    Point a = CURR(P,i) - p, b = NEXT(P,i) - p;
    if (imag(a) > imag(b)) swap(a, b);
    if (imag(a) <= 0 && 0 < imag(b))
      if (cross(a, b) < 0) in = !in;
    if (cross(a, b) == 0 && dot(a, b) <= 0) return ON;
  }
  return in ? IN : OUT;
}

double ReadFrac(FILE *file) {
  fscanf(file, " ");
  long long a;
  long long b = 1;
  int v = fscanf(file, "%lld", &a);
  int c = getc(file);
  assert(c >= 0);
  if (c != '/') {
    ungetc(c, file);
  } else {
    v = fscanf(file, "%lld", &b);
  }
  return (double)a / b;
}
Point ReadPoint(FILE *file) {
  double x = ReadFrac(file);
  int v = fscanf(file, " ,");
  assert(v == 0);
  double y = ReadFrac(file);
  return Point(x, y);
}

struct Input {
  std::vector<Polygon> silhouettes;
  std::vector<Line> skeltons;
  bool ReadInput(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == nullptr) {
      fprintf(stderr, "%s can't open!\n", filename);
      exit(1);
    }
    ReadInput(file);
    fclose(file);
    return true;
  }
  bool ReadInput(FILE *file) {
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
  bool ConatainSilhouette(const Point &p) const {
    bool ret = false;
    for (Polygon silhouette : silhouettes) {
      if (Area(silhouette) < 0) {
        // 反転している箇所に含まれる場合は常に穴なので即返す
        reverse(silhouette.begin(), silhouette.end());
        if (Contains(silhouette, p) == IN) { return false; }
      } else {
        if (Contains(silhouette, p) == IN) { ret = true; }
      }
    }
    return ret;
  }
  std::vector<double> MinMaxXY() const {
    std::vector<double> ret = { 1e+10, 1e+10, -1e+10, -1e+10 };
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
};

struct Output {
  std::vector<Point> source_points;
  std::vector<std::vector<int>> facet_indecies;
  std::vector<Point> dest_points;
  std::vector<Polygon> facet_polygons;
  bool ReadOutput(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == nullptr) {
      fprintf(stderr, "%s can't open!\n", filename);
      exit(1);
    }
    ReadOutput(file);
    fclose(file);
    return true;
  }
  bool ReadOutput(FILE *file) {
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
  bool ConatainFacet(const Point &p) const {
    for (Polygon facet : facet_polygons) {
      if (Area(facet) < 0) {
        reverse(facet.begin(), facet.end());
      }
      if (Contains(facet, p) == IN) { return true; }
    }
    return false;
  }
  std::vector<double> MinMaxXY() const {
    std::vector<double> ret = { 1e+10, 1e+10, -1e+10, -1e+10 };
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
};

struct Random {
  unsigned int x;
  unsigned int y;
  unsigned int z;
  unsigned int w; 
  Random() : x(0x34fb2383), y(0x327328fa), z(0xabd4b54a), w(0xa9dba8d1) {;}
  Random(int s) : x(0x34fb2383), y(0x327328fa), z(0xabd4b54a), w(s) {
    for (int i = 0; i < 100; i++) { Xor128(); }
  }
  void Seed(int s) {
    *this = Random(s);
  }
  unsigned int Xor128() {
    unsigned int t;
    t = x ^ (x << 11);
    x = y; y = z; z = w;
    return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)); 
  }
  int next(int r) { return Xor128() % r; }
  int next(int l, int r) { return next(r - l + 1) + l; }
  long long next(long long r) { return (long long)((((unsigned long long)Xor128() << 32) + (unsigned long long)Xor128()) % r); }
  long long next(long long l, long long r) { return next(r - l + 1) + l; }
  double next(double r) { return (double)Xor128() / 0xffffffff * r; }
  double next(double l, double r) { return next(r - l) + l; }
};

void Usage() {
  fprintf(stderr, "input_filename solution_filename\n");
  exit(1);
}

int main(int argc, char* argv[]) {
  if (argc != 3) { Usage(); }
  const char* input_filename = argv[1];
  const char* output_filename = argv[2];
  Input input;
  Output output;
  input.ReadInput(input_filename);
  output.ReadOutput(output_filename);
  std::vector<double> min_max_xy;
  {
    std::vector<double> min_max_xy1 = input.MinMaxXY();
    std::vector<double> min_max_xy2 = output.MinMaxXY();
    min_max_xy.push_back(std::min(min_max_xy1[0], min_max_xy2[0]));
    min_max_xy.push_back(std::min(min_max_xy1[1], min_max_xy2[1]));
    min_max_xy.push_back(std::max(min_max_xy1[2], min_max_xy2[2]));
    min_max_xy.push_back(std::max(min_max_xy1[3], min_max_xy2[3]));
  }

  Random rnd(123354);
  int area_and = 0;
  int area_or = 0;
  for (int i = 0; i < MONTE_COUNT; i++) {
    double x = rnd.next(min_max_xy[0], min_max_xy[2]);
    double y = rnd.next(min_max_xy[1], min_max_xy[3]);
    Point p(x, y);
    bool contain_input = input.ConatainSilhouette(p);
    bool contain_output = output.ConatainFacet(p);
    if (contain_input && contain_output) {
      area_and++;
    }
    if (contain_input || contain_output) {
      area_or++;
    }
  }
  // std::cout << area_and << " " << area_or << " " << std::endl;
  fprintf(stdout, "%.6f\n", (double)area_and / area_or);
  return 0;
}
