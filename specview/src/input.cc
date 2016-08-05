#include "input.h"
#include <iostream>
using namespace std;

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

Point ReadPoint(string s) {
  size_t comma = s.find(',');
  if (comma == string::npos) {
    cerr << "No comma in the line: " << s << "\n";
    exit(1);
  }
  mpq_class x = ReadFraction(s.substr(0, comma));
  mpq_class y = ReadFraction(s.substr(comma + 1));
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
