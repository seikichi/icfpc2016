#include <iostream>
#include <complex>
#include <vector>
#include <cstdlib>
using namespace std;

using Point = complex<double>;
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

double ReadFraction(string s) {
  size_t slash = s.find('/');
  if (slash == string::npos) {
    return stoi(s);
  } else {
    double numer = stoi(s.substr(0, slash));
    double denom = stoi(s.substr(slash+1));
    return numer / denom;
  }
}

Point ReadPoint(string line) {
  size_t comma = line.find(',');
  if (comma == string::npos) {
    cerr << "No comma in the line: " << line << "\n";
    exit(1);
  }
  double x = ReadFraction(line.substr(0, comma));
  double y = ReadFraction(line.substr(comma+1));
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
    Point second = ReadPoint(line.substr(space+1));
    skeleton.push_back(Segment(first, second));
  }
  return skeleton;
}

int main() {
  Silhouette silhoutte = ReadSilhouette();
  Skeleton skeleton = ReadSkeleton();

  const string header = R"(<svg xmlns="http://www.w3.org/2000/svg" viewbox="-0.1 -0.1 1.1 1.1">)";
  cout << header << "\n";

  string path;
  for (auto& polygon : silhoutte) {
    bool first = true;
    for (auto& point : polygon) {
      if (first) {
        path += "M";
        first = false;
      } else {
        path += "L";
      }
      path += to_string(point.real()) + " " + to_string(point.imag()) + " ";
    }
    path += "z ";
  }
  cout << "<path d=\"" << path << "\" />\n";

  cout << "</svg>\n";
  return 0;
}
