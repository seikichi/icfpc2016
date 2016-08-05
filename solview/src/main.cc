#include "out.h"

#include <gmpxx.h>
#include <complex>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

void Usage() {
  fprintf(stderr, "Usage: ./solview source|dest < sample.out\n");
  exit(1);
}

void Canonicalize(Output &output, Polygon &square) {
  mpq_class xg(0), yg(0);
  int count = 0;
  for (auto& point : output.dest_points) {
    xg += point.real();
    yg += point.imag();
    count++;
  }
  xg /= count;
  yg /= count;
  for (auto& point : output.dest_points) {
    point.real() -= xg;
    point.imag() -= yg;
  }
  for (auto& point : square) {
    point.real() -= xg;
    point.imag() -= yg;
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    Usage();
  }
  Output output;
  output.ReadOutput(stdin);

  const string header = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="-1.5 -1.5 3 3">)";
  cout << header << "\n";
  cout << "<g transform=\"scale(1, -1)\">\n";
  if (strcmp(argv[1], "source") == 0) {
    const string square = R"STR(<path d="M0.000000 0.000000 L1.000000 0.000000 L1.000000 1.000000 L0.000000 1.000000  z" fill="rgb(128,128,128)" stroke="rgb(0,0,0)" stroke-width="0.01" />)STR";
    cout << square << "\n" << endl;
    for (auto& facet : output.facet_indecies) {
      int n = facet.size();
      for (int i = 0; i < n; i++) {
        Point &s = output.source_points[facet[i]];
        Point &e = output.source_points[facet[(i + 1) % n]];
        cout << "<line x1=\"" << s.real().get_d() << "\" y1=\""
          << s.imag().get_d() << "\" x2=\""
          << e.real().get_d() << "\" y2=\""
          << e.imag().get_d()
          << "\" style=\"stroke:rgb(0,0,0);stroke-width:0.01\" />\n";
      }
    }
  } else {
    Polygon square = {Point(0, 0), Point(1, 0), Point(1, 1), Point(0, 1)};
    Canonicalize(output, square);
    string path;
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

    path = "";
    for (auto& facet : output.facet_indecies) {
      bool first = true;
      int n = facet.size();
      for (int i = 0; i < n; i++) {
        if (first) {
          path += "M";
          first = false;
        } else {
          path += "L";
        }
        Point &p = output.dest_points[facet[i]];
        path += to_string(p.real().get_d()) + " ";
        path += to_string(p.imag().get_d()) + " ";
      }
      path += "z ";
      cout << "<path d=\"" << path << "\" fill=\"rgb(245,195,195)\" stroke=\"rgb(255,128,128)\" stroke-width=\"0.01\" />\n";
    }
  }
  cout << "</g>\n";
  cout << "</svg>\n";
  return 0;
}
