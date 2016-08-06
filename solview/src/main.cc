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

// void Canonicalize(Output &output, Polygon &square) {
//   mpq_class xg(0), yg(0);
//   int count = 0;
//   for (auto& point : output.dest_points) {
//     xg += point.real();
//     yg += point.imag();
//     count++;
//   }
//   xg /= count;
//   yg /= count;
//   for (auto& point : output.dest_points) {
//     point.real() -= xg;
//     point.imag() -= yg;
//   }
//   for (auto& point : square) {
//     point.real() -= xg;
//     point.imag() -= yg;
//   }
// }

int main(int argc, char *argv[]) {
  if (argc != 2) {
    Usage();
  }
  Output output;
  output.ReadOutput(stdin);

  if (strcmp(argv[1], "source") == 0) {
    output.WriteSVGSource(stdout);
  } else {
    output.WriteSVGDest(stdout);
  }
  return 0;
}
