#include <gmpxx.h>
#include <complex>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

struct Point {
  mpq_class x, y;
  Point(const mpq_class& x, const mpq_class& y) : x(x), y(y) {}
};
using Polygon = vector<Point>;
using Silhouette = vector<Polygon>;
using Segment = pair<Point, Point>;
using Skeleton = vector<Segment>;

int ReadIntegerLine() {
  string line;
  if (!getline(cin, line)) {
    cerr << "Failed to read integer: " << line << endl;
    exit(1);
  }
  return stoi(line);
}

mpq_class ReadFraction(string s) {
  size_t slash = s.find('/');
  if (slash == string::npos) {
    mpz_class i(s, 10);
    return mpq_class(i, 1);
  } else {
    mpz_class numer(s.substr(0, slash), 10);
    mpz_class denom(s.substr(slash + 1), 10);
    return mpq_class(numer, denom);
  }
}

Point ReadPoint(string line) {
  size_t comma = line.find(',');
  if (comma == string::npos) {
    cerr << "No comma in the line: " << line << "\n";
    exit(1);
  }
  mpq_class x = ReadFraction(line.substr(0, comma));
  mpq_class y = ReadFraction(line.substr(comma + 1));
  return Point(x, y);
}

Polygon ReadPolygon() {
  Polygon polygon;
  int n = ReadIntegerLine();
  string line;
  for (int i = 0; i < n; ++i) {
    if (!getline(cin, line)) {
      cerr << "Failed to read a coordiate of the vertex\n";
      exit(1);
    }
    polygon.push_back(ReadPoint(line));
  }
  return polygon;
}

Silhouette ReadSilhouette() {
  int n = ReadIntegerLine();
  Silhouette silhuette;
  for (int i = 0; i < n; ++i) {
    silhuette.push_back(ReadPolygon());
  }
  return silhuette;
}

Skeleton ReadSkeleton() {
  int n = ReadIntegerLine();
  Skeleton skeleton;
  string line;
  for (int i = 0; i < n; ++i) {
    if (!getline(cin, line)) {
      cerr << "Failed to read the segment of skeleton\n";
      exit(1);
    }
    size_t space = line.find(' ');
    Point first = ReadPoint(line.substr(0, space));
    Point second = ReadPoint(line.substr(space + 1));
    skeleton.push_back(Segment(first, second));
  }
  return skeleton;
}

void Canonicalize(Silhouette& silhouette, Skeleton& skeleton, Polygon& square) {
  mpq_class xg(0), yg(0);
  int count = 0;
  for (auto& polygon : silhouette) {
    for (auto& point : polygon) {
      xg += point.x;
      yg += point.y;
      count++;
    }
  }
  xg /= count;
  yg /= count;
  for (auto& polygon : silhouette) {
    for (auto& point : polygon) {
      point.x -= xg;
      point.y -= yg;
    }
  }
  for (auto& segment : skeleton) {
    segment.first.x -= xg;
    segment.first.y -= yg;
    segment.second.x -= xg;
    segment.second.y -= yg;
  }
  for (auto& point : square) {
    point.x -= xg;
    point.y -= yg;
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
      path +=
          to_string(point.x.get_d()) + " " + to_string(point.y.get_d()) + " ";
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
    path += to_string(point.x.get_d()) + " " + to_string(point.y.get_d()) + " ";
  }
  cout << "<path d=\"" << path
       << " z\" fill=\"none\" "
          "stroke=\"rgb(128,128,128)\" stroke-width=\"0.01\" />\n";

  for (auto& segment : skeleton) {
    cout << "<line x1=\"" << segment.first.x.get_d() << "\" y1=\""
         << segment.first.y.get_d() << "\" x2=\"" << segment.second.x.get_d()
         << "\" y2=\"" << segment.second.y.get_d()
         << "\" style=\"stroke:rgb(246,147,147);stroke-width:0.01\" />\n";
  }

  cout << "</svg>\n";
  return 0;
}
