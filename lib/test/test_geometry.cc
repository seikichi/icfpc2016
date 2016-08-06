#include "gtest/gtest.h"
#include "geometry.h"

#include <iostream>
using namespace std;

TEST(ConvexCut, Easy) {
  //  ------
  //  |    | <= rest 
  //  |    |
  //  -----> 
  //  |    | <= discard
  //  ------
  Polygon ps;
  ps.push_back(Point(mpq_class(0, 1), mpq_class(0, 1)));
  ps.push_back(Point(mpq_class(1, 1), mpq_class(0, 1)));
  ps.push_back(Point(mpq_class(1, 1), mpq_class(1, 1)));
  ps.push_back(Point(mpq_class(0, 1), mpq_class(1, 1)));

  Line l(
      Point(mpq_class(0, 1), mpq_class(1, 3)),
      Point(mpq_class(1, 1), mpq_class(1, 3)));

  Polygon poly = ConvexCut(ps, l);

  EXPECT_EQ(Point(mpq_class(1, 1), mpq_class(1, 3)), poly[0]);
  EXPECT_EQ(Point(mpq_class(1, 1), mpq_class(1, 1)), poly[1]);
  EXPECT_EQ(Point(mpq_class(0, 1), mpq_class(1, 1)), poly[2]);
  EXPECT_EQ(Point(mpq_class(0, 1), mpq_class(1, 3)), poly[3]);
}

TEST(GetSourcePoint, Easy) {
  // .<=d2
  // |
  // |
  // .<=d3
  // .<=d1
  // 
  // ..--.<=s2
  // ^^=s3
  // =s1
  Point d1(mpq_class(0, 1), mpq_class(1, 1));
  Point d2(mpq_class(0, 1), mpq_class(2, 1));
  Point d3(mpq_class(0, 1), mpq_class(4, 3));
  Point s1(mpq_class(0, 1), mpq_class(0, 1));
  Point s2(mpq_class(1, 1), mpq_class(0, 1));

  Point s3 = GetSourcePoint(d1, d2, d3, s1, s2);
  EXPECT_EQ(Point(mpq_class(1, 3), mpq_class(0, 1)), s3);
}
