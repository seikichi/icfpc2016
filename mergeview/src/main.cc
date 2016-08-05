#include "in.h"
#include "out.h"

#include <gmpxx.h>
#include <complex>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

void Usage() {
  fprintf(stderr, "Usage: ./mergeview sample.in sample.out\n");
  exit(1);
}

void Canonicalize(Input &input, Output &output, Polygon &square) {
  mpq_class xg(0), yg(0);
  int count = 0;
  for (auto& polygon : input.silhouettes) {
    for (auto &point : polygon) {
      xg += point.real();
      yg += point.imag();
      count++;
    }
  }
  for (auto& point : output.dest_points) {
    xg += point.real();
    yg += point.imag();
    count++;
  }
  xg /= count;
  yg /= count;
  input.MakeSilhouettesD(Point(xg, yg));
  output.MakeFacetD(Point(xg, yg));
  for (auto& segment : input.skeltons) {
    segment[0] -= Point(xg, yg);
    segment[1] -= Point(xg, yg);
  }
  for (auto& point : square) {
    point.real() -= xg;
    point.imag() -= yg;
  }
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    Usage();
  }
  Input input;
  Output output;
  Polygon square = {Point(0, 0), Point(1, 0), Point(1, 1), Point(0, 1)};
  input.ReadInput(argv[1]);
  output.ReadOutput(argv[2]);
  Canonicalize(input, output, square);

  const string header = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="-1.5 -1.5 3 3">)";
  cout << header << "\n";
  cout << "<g transform=\"scale(1, -1)\">\n";

  // square
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

  // input
  path = "";
  vector<string> flip_path;
  for (int i = 0; i < (int)input.silhouettes.size(); i++) {
    bool first = true;
    for (auto& p : input.silhouettes_d[i]) {
      if (first) {
        path += "M";
        first = false;
      } else {
        path += "L";
      }
      path += to_string(p.real()) + " ";
      path += to_string(p.imag()) + " ";
    }
    path += "z ";
    // if (input.flip_flags[i]) {
    //   flip_path.push_back(path);
    // } else {
    cout << "<path d=\"" << path << "\" fill=\"rgba(245,195,195,1.0)\" stroke=\"rgb(255,128,128)\" stroke-width=\"0.01\" />\n";
    // }
  }
  // for (auto& path : flip_path) {
  //   cout << "<path d=\"" << path << "\" fill=\"rgb(255,255,255)\" stroke=\"rgb(255,128,128)\" stroke-width=\"0.01\" />\n";
  // }
  for (auto& segment : input.skeltons) {
    cout << "<line x1=\"" << segment[0].real().get_d() << "\" y1=\""
         << segment[0].imag().get_d() << "\" x2=\""
         << segment[1].real().get_d() << "\" y2=\""
         << segment[1].imag().get_d()
         << "\" style=\"stroke:rgb(246,147,147);stroke-width:0.01\" />\n";
  }

  // output
  path = "";
  for (auto& facet : output.facet_polygons_d) {
    bool first = true;
    for (auto& p : facet) {
      if (first) {
        path += "M";
        first = false;
      } else {
        path += "L";
      }
      path += to_string(p.real()) + " ";
      path += to_string(p.imag()) + " ";
    }
    path += "z ";
    cout << "<path d=\"" << path << "\" fill=\"rgba(195,195,245,0.3)\" stroke=\"rgb(128,128,255)\" stroke-width=\"0.01\" />\n";
  }
  cout << "</g>\n";
  cout << "</svg>\n";
  return 0;
}
