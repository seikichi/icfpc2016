#include "crease.h"
#include "gtest/gtest.h"
using namespace std;

TEST(CalculateCreases, FoldOnce) {
  vector<pair<mpq_class, mpq_class>> expected = {
    make_pair(mpq_class(0, 1), mpq_class(0, 1)),
    make_pair(mpq_class(2, 3), mpq_class(2, 3)),
    make_pair(mpq_class(1, 1), mpq_class(1, 3)),
  };
  EXPECT_EQ(expected, CalculateCreases(1, mpq_class(2, 3)));
}

TEST(CalculateCreases, FoldTwice) {
  vector<pair<mpq_class, mpq_class>> expected = {
    make_pair(mpq_class(0, 1), mpq_class(0, 1)),
    make_pair(mpq_class(1, 3), mpq_class(1, 3)),
    make_pair(mpq_class(1, 2), mpq_class(1, 6)),
    make_pair(mpq_class(2, 3), mpq_class(1, 3)),
    make_pair(mpq_class(1, 1), mpq_class(0, 1)),
  };
  EXPECT_EQ(expected, CalculateCreases(1, mpq_class(1, 3)));
}

TEST(CalculateCreases, FoldThreeTimes) {
  vector<pair<mpq_class, mpq_class>> expected = {
    make_pair(mpq_class(0, 1), mpq_class(0, 1)),
    make_pair(mpq_class(1, 6), mpq_class(1, 6)),
    make_pair(mpq_class(1, 4), mpq_class(1, 12)),
    make_pair(mpq_class(1, 3), mpq_class(1, 6)),
    make_pair(mpq_class(1, 2), mpq_class(0, 1)),
    make_pair(mpq_class(2, 3), mpq_class(1, 6)),
    make_pair(mpq_class(3, 4), mpq_class(1, 12)),
    make_pair(mpq_class(5, 6), mpq_class(1, 6)),
    make_pair(mpq_class(1, 1), mpq_class(0, 1)),
  };
  EXPECT_EQ(expected, CalculateCreases(1, mpq_class(1, 6)));
}
