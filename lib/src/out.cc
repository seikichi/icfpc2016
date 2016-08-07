#include "out.h"

#include "inout.h"
#include <assert.h>
#include <sstream>
#include <iostream>

void Output::Init() {
  source_points = {
    Point(mpq_class(0, 1), mpq_class(0, 1)),
    Point(mpq_class(1, 1), mpq_class(0, 1)),
    Point(mpq_class(1, 1), mpq_class(1, 1)),
    Point(mpq_class(0, 1), mpq_class(1, 1)),
  };
  dest_points = source_points;
  facet_indecies.push_back({ 0, 1, 2, 3 });
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
    v = fscanf(file, " %d", &n);
    assert(v == 1);
    facet_indecies[i].resize(n);
    for (int j = 0; j < n; j++) {
      int index;
      v = fscanf(file, "%d", &index);
      assert(v == 1);
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
  bool result = WriteOutput(file);
  fclose(file);
  return result;
}
bool Output::WriteOutput(FILE *file) const {
  std::string str = WriteString();
  fprintf(file, "%s", str.c_str());
  return Validate();
}
std::string Output::WriteString() const {
  std::string ret;
  // source positions part
  ret += std::to_string(source_points.size()) + "\n";
  for (auto &p : source_points) {
    ret += p.real().get_str() + "," + p.imag().get_str() + "\n";
    if (ret.size() > 5000) { return ret; }
  }
  // facets part
  ret += std::to_string(facet_indecies.size()) + "\n";
  for (auto &facets : facet_indecies) {
    ret += std::to_string(facets.size());
    for (int index : facets) {
      ret += " " + std::to_string(index);
      if (ret.size() > 5000) { return ret; }
    }
    ret += "\n";
  }
  // destination positions part
  for (auto &p : dest_points) {
    ret += p.real().get_str() + "," + p.imag().get_str() + "\n";
    if (ret.size() > 5000) { return ret; }
  }
  return ret;
}
bool Output::Validate() const {
  bool ret = true;
  for (int i = 0; i < (int)source_points.size(); i++) {
    for (int j = 0; j < i; j++) {
      if (source_points[i] == source_points[j]) {
        fprintf(stderr, "Invalid solution spec: duplicate source point %d %d.\n", i, j);
        std::cerr << source_points[i] << std::endl;
        ret = false;
      }
    }
  }
  if (WriteString().size() > 5000) {
    fprintf(stderr, "Invalid solution spec: Output size is larger than 5000B.\n");
    ret = false;
  }
  return ret;
}


bool Output::WriteSVGSource(const char *filename) const {
  FILE *file = fopen(filename, "w");
  if (file == nullptr) {
    fprintf(stderr, "%s can't open!\n", filename);
    exit(1);
  }
  bool result = WriteSVGSource(file);
  fclose(file);
  return result;
}
bool Output::WriteSVGSource(FILE *file) const {
  std::stringstream sout;
  const std::string header = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="-1.5 -1.5 3 3">)";
  sout << header << "\n";
  sout << "<g transform=\"scale(1, -1)\">\n";
  const std::string square = R"STR(<path d="M0.000000 0.000000 L1.000000 0.000000 L1.000000 1.000000 L0.000000 1.000000  z" fill="rgb(128,128,128)" stroke="rgb(0,0,0)" stroke-width="0.01" />)STR";
  sout << square << "\n" << std::endl;
  for (auto& facet : facet_indecies) {
    int n = facet.size();
    for (int i = 0; i < n; i++) {
      const Point &s = source_points[facet[i]];
      const Point &e = source_points[facet[(i + 1) % n]];
      sout << "<line x1=\"" << s.real().get_d() << "\" y1=\""
        << s.imag().get_d() << "\" x2=\""
        << e.real().get_d() << "\" y2=\""
        << e.imag().get_d()
        << "\" style=\"stroke:rgb(0,0,0);stroke-width:0.01\" />\n";
    }
  }
  sout << "</g>\n";
  sout << "</svg>\n";
  fprintf(file, "%s", sout.str().c_str());
  return true;
}
bool Output::WriteSVGDest(const char *filename) const {
  FILE *file = fopen(filename, "w");
  if (file == nullptr) {
    fprintf(stderr, "%s can't open!\n", filename);
    exit(1);
  }
  bool result = WriteSVGDest(file);
  fclose(file);
  return result;
}
bool Output::WriteSVGDest(FILE *file) const {
  mpq_class xg(0), yg(0);
  int count = 0;
  for (const auto& point : dest_points) {
    xg += point.real();
    yg += point.imag();
    count++;
  }
  xg /= count;
  yg /= count;
  Point offset(xg, yg);

  std::stringstream sout;
  const std::string header = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="-1.5 -1.5 3 3">)";
  sout << header << "\n";
  sout << "<g transform=\"scale(1, -1)\">\n";
  Polygon square = {Point(0, 0), Point(1, 0), Point(1, 1), Point(0, 1)};

  std::string path;
  bool first = true;
  for (auto point : square) {
    point -= offset;
    if (first) {
      path += "M";
      first = false;
    } else {
      path += "L";
    }
    path += std::to_string(point.real().get_d()) + " ";
    path += std::to_string(point.imag().get_d()) + " ";
  }
  sout << "<path d=\"" << path
    << " z\" fill=\"none\" "
    "stroke=\"rgb(128,128,128)\" stroke-width=\"0.01\" />\n";

  path = "";
  for (auto& facet : facet_indecies) {
    bool first = true;
    int n = facet.size();
    for (int i = 0; i < n; i++) {
      if (first) {
        path += "M";
        first = false;
      } else {
        path += "L";
      }
      Point p = dest_points[facet[i]];
      p -= offset;
      path += std::to_string(p.real().get_d()) + " ";
      path += std::to_string(p.imag().get_d()) + " ";
    }
    path += "z ";
    sout << "<path d=\"" << path << "\" fill=\"rgb(245,195,195)\" stroke=\"rgb(255,128,128)\" stroke-width=\"0.01\" />\n";
  }
  sout << "</g>\n";
  sout << "</svg>\n";
  fprintf(file, "%s", sout.str().c_str());
  return true;
}

void Output::MakeFacetD(const Point &offset) const {
  // make facet polygons
  facet_polygons_d.clear();
  facet_rects.clear();
  for (auto indices : facet_indecies) {
    PolygonD facet;
    std::vector<double> rect = { 1e+10, 1e+10, -1e+10, -1e+10 };
    for (int index : indices) {
      PointD p = mpq2d(dest_points[index] - offset);
      facet.push_back(p);
      rect[0] = std::min(rect[0], p.real());
      rect[1] = std::min(rect[1], p.imag());
      rect[2] = std::max(rect[2], p.real());
      rect[3] = std::max(rect[3], p.imag());
    }
    if (Area(facet) < 0) {
      reverse(facet.begin(), facet.end());
    }
    facet_polygons_d.push_back(facet);
    facet_rects.push_back(rect);
  }
  // make facet convex
  facet_convex.clear();
  for (const auto &p : dest_points) {
    facet_convex.push_back(mpq2d(p - offset));
  }
  facet_convex = ConvexHull(facet_convex);
}
bool Output::ContainFacet(const PointD &p) const {
  if (Contains(facet_convex, p) != ContainResult::IN) { return false; }
  for (int i = 0; i < (int)facet_polygons_d.size(); i++) {
    const auto &rect = facet_rects[i];
    const auto &facet = facet_polygons_d[i];
    if (rect[0] < p.real() && p.real() < rect[2] && rect[1] < p.imag() && p.imag() < rect[3]) {
      if (Contains(facet, p) == ContainResult::IN) { return true; }
    }
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
