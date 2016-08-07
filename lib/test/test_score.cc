#include "gtest/gtest.h"
#include "score.h"
#include <iostream>
using namespace std;

TEST(ScoringMonte, RoughCheck) {
  double score = ScoringMonte("../problems/50.in", "./test/files/sample.out", 10000);
  EXPECT_NEAR(0.0245098, score, 1e-5);
}

TEST(ScoringMonte, RoughCheck2) {
  double score = ScoringMonte("../problems/86.in", "./test/files/86.out", 10000);
  EXPECT_NEAR(0.8604818, score, 1e-5);
}

TEST(ScoringMonte, Same) {
  double score = ScoringMonte("../problems/25.in", "../hand_solution/25.out", 10000);
  EXPECT_EQ(1.0, score);
}

// TEST(ScoringMonte, PerfectHit) {
//   bool hit1 = PerfectHit("../problems/86.in", "./test/files/86.out", 100);
//   EXPECT_EQ(false, hit1);
//
//   bool hit2 = PerfectHit("../problems/25.in", "../hand_solution/25.out", 100);
//   EXPECT_EQ(true, hit2);
// }
