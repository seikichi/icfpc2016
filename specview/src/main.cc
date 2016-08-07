#include <gmpxx.h>
#include <complex>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "input.h"
using namespace std;

void Canonicalize(Silhouette& silhouette, Skeleton& skeleton, Polygon& square) {
  mpq_class xg(0), yg(0);
  int count = 0;
  for (auto& polygon : silhouette) {
    for (auto& point : polygon) {
      xg += point.real();
      yg += point.imag();
      count++;
    }
  }
  xg /= count;
  yg /= count;
  for (auto& polygon : silhouette) {
    for (auto& point : polygon) {
      point = Point(point.real() - xg, point.imag() - yg);
    }
  }
  for (auto& segment : skeleton) {
    segment.first = Point(segment.first.real() - xg, segment.first.imag() - yg);
    segment.second = Point(segment.second.real() - xg, segment.second.imag() - yg);
  }
  for (auto& point : square) {
    point = Point(point.real() - xg, point.imag() - yg);
  }
}

int main() {
  Silhouette silhouette = ReadSilhouette();
  Skeleton skeleton = ReadSkeleton();
  Polygon square = {Point(0, 0), Point(1, 0), Point(1, 1), Point(0, 1)};
  Canonicalize(silhouette, skeleton, square);

  const string header =
      R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="-1.5 -1.5 3 3">)";
  cout << header << "\n";
  cout << "<g transform=\"scale(1, -1)\">\n";

  string path;
  for (auto& polygon : silhouette) {
    bool first = true;
    for (auto& point : polygon) {
      if (first) {
        path += "M";
        first = false;
      } else {
        path += "L";
      }
      path += to_string(point.real().get_d()) + " ";
      path += to_string(point.imag().get_d()) + " ";
    }
    path += "z ";
  }
  cout << "<path d=\"" << path << "\" fill=\"rgb(245,195,195)\" />\n";

  path = "";
  bool first = true;
  for (auto& point : square) {
    if (first) {
      path += "M";
      first = false;
    } else {
      path += "L";
    }
    path += to_string(point.real().get_d()) + " ";
    path += to_string(point.imag().get_d()) + " ";
  }
  cout << "<path d=\"" << path
       << " z\" fill=\"none\" "
          "stroke=\"rgb(128,128,128)\" stroke-width=\"0.01\" />\n";

  for (auto& segment : skeleton) {
    cout << "<line x1=\"" << segment.first.real().get_d() << "\" y1=\""
         << segment.first.imag().get_d() << "\" x2=\""
         << segment.second.real().get_d() << "\" y2=\""
         << segment.second.imag().get_d()
         << "\" style=\"stroke:rgb(246,147,147);stroke-width:0.01\" />\n";
  }

  cout << "</g>\n";
  cout << "</svg>\n";
  return 0;
}
