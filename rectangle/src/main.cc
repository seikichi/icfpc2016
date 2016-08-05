#include "input.h"
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
  if (width < mpq_class(1, 2))
    width = mpq_class(1, 2);
  if (width > 1)
    width = mpq_class(1, 1);
  width.canonicalize();
  mpq_class height = max_y - min_y;
  if (height < mpq_class(1, 2))
    height = mpq_class(1, 2);
  if (height > 1)
    height = mpq_class(1, 1);
  height.canonicalize();

  vector<mpq_class> xs, ys;
  if (width == mpq_class(1, 1))
    xs = {0, 1};
  else
    xs = {0, width, 1};
  if (height == mpq_class(1, 1))
    ys = {0, 1};
  else
    ys = {0, height, 1};

  // source positions part
  cout << xs.size() * ys.size() << "\n";
  for (auto& y : ys) {
    for (auto& x : xs) {
      cout << PointToString(Point(x, y)) << "\n";
    }
  }
  // facets part
  cout << (xs.size()-1)*(ys.size()-1) << "\n";
  for (int i = 0; i < (int)ys.size()-1; ++i) {
    for (int j = 0; j < (int)xs.size()-1; ++j) {
      cout << "4 "
           << (i*xs.size() + j) << " "
           << (i*xs.size() + j+1) << " "
           << ((i+1)*xs.size() + j+1) << " "
           << ((i+1)*xs.size() + j) << "\n";
    }
  }
  // destination positions part
  mpq_class offset_x = xg - width/2;
  mpq_class offset_y = yg - height/2;
  for (auto& y : ys) {
    mpq_class dest_y = (ys.size() == 3 && y == 1) ? (2*height-1) : y;
    for (auto& x : xs) {
      mpq_class dest_x = (xs.size() == 3 && x == 1) ? (2*width-1) : x;
      cout << PointToString(Point(dest_x + offset_x, dest_y + offset_y)) << "\n";
    }
  }
}
