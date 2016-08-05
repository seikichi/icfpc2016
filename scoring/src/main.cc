#include "geometry.h"
#include "inout.h"
#include "score.h"

#include <assert.h>

static int MONTE_COUNT = 10000;

void Usage() {
  fprintf(stderr, "input_filename solution_filename\n");
  exit(1);
}

int main(int argc, char* argv[]) {
  if (argc != 3) { Usage(); }
  const char* input_filename = argv[1];
  const char* output_filename = argv[2];
  double ans = ScoringMonte(input_filename, output_filename, MONTE_COUNT);
  fprintf(stdout, "%.6f\n", ans);
  return 0;
}
