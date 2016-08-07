#include "gtest/gtest.h"
#include "score.h"
#include <iostream>
using namespace std;

TEST(ScoringMonte, RoughCheck) {
  double score = ScoringMonte("../problems/50.in", "./test/files/sample.out", 1000);
  EXPECT_NEAR(0.0254902, score, 1e-5);
}

TEST(ScoringMonte, RoughCheck2) {
  double score = ScoringMonte("../problems/86.in", "./test/files/86.out", 1000);
  EXPECT_NEAR(0.873684, score, 1e-5);
}

TEST(ScoringMonte, Same) {
  double score = ScoringMonte("../problems/25.in", "../hand_solution/25.out", 1000);
  EXPECT_EQ(1.0, score);
}
