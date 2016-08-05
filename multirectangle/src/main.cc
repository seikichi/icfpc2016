#include "input.h"
#include "crease.h"
#include <iostream>
using namespace std;

string FractionToString(mpq_class f) {
  f.canonicalize();
  if (f.get_den() == 1)
    return f.get_num().get_str();
  else
    return f.get_num().get_str() + "/" + f.get_den().get_str();
}

string PointToString(Point p) {
  return FractionToString(p.real()) + "," + FractionToString(p.imag());
}

int main() {
  Silhouette silhouette = ReadSilhouette();
  Skeleton skeleton = ReadSkeleton();

  mpq_class xg(0, 1);
  mpq_class yg(0, 1);
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

  mpq_class min_x = silhouette[0][0].real();
  mpq_class max_x = silhouette[0][0].real();
  mpq_class min_y = silhouette[0][0].imag();
  mpq_class max_y = silhouette[0][0].imag();

  for (auto& polygon : silhouette) {
    for (auto& point : polygon) {
      if (point.real() < min_x) min_x = point.real();
      if (point.real() > max_x) max_x = point.real();
      if (point.imag() < min_y) min_y = point.imag();
      if (point.imag() > max_y) max_y = point.imag();
    }
  }

  mpq_class width = max_x - min_x;
  width.canonicalize();
  mpq_class height = max_y - min_y;
  height.canonicalize();

  vector<pair<mpq_class, mpq_class>> x_creases = CalculateCreases(1, width);
  vector<pair<mpq_class, mpq_class>> y_creases = CalculateCreases(1, height);

  // source positions part
  cout << x_creases.size() * y_creases.size() << "\n";
  for (auto& y : y_creases) {
    for (auto& x : x_creases) {
      cout << PointToString(Point(x.first, y.first)) << "\n";
    }
  }
  // facets part
  cout << (x_creases.size()-1)*(y_creases.size()-1) << "\n";
  for (int i = 0; i < (int)y_creases.size()-1; ++i) {
    for (int j = 0; j < (int)x_creases.size()-1; ++j) {
      cout << "4 "
           << (i*x_creases.size() + j) << " "
           << (i*x_creases.size() + j+1) << " "
           << ((i+1)*x_creases.size() + j+1) << " "
           << ((i+1)*x_creases.size() + j) << "\n";
    }
  }
  // destination positions part
  mpq_class offset_x = xg - min(width, mpq_class(1)) / 2;
  mpq_class offset_y = yg - min(height, mpq_class(1)) / 2;
  for (auto& y : y_creases) {
    for (auto& x : x_creases) {
      cout << PointToString(Point(x.second + offset_x, y.second + offset_y)) << "\n";
    }
  }
}
