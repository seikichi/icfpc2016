#include "gtest/gtest.h"
#include "geometry.h"

#include <iostream>
using namespace std;

TEST(SameLine, Check) {
  Line l1(Point(mpq_class(0, 1), mpq_class(1, 2)), Point(mpq_class(1, 1), mpq_class(1, 2)));
  Line l1_same(Point(mpq_class(3, 1), mpq_class(1, 2)), Point(mpq_class(4, 1), mpq_class(1, 2)));
  Line l1_same2(Point(mpq_class(4, 1), mpq_class(1, 2)), Point(mpq_class(3, 1), mpq_class(1, 2)));
  Line l1_parallel(Point(mpq_class(0, 1), mpq_class(3, 2)), Point(mpq_class(1, 1), mpq_class(3, 2)));
  Line l2(Point(mpq_class(1, 2), mpq_class(0, 1)), Point(mpq_class(1, 2), mpq_class(1, 1)));
  Line l3(Point(mpq_class(1, 2), mpq_class(1, 1)), Point(mpq_class(1, 2), mpq_class(0, 1)));

  EXPECT_EQ(true, SameLine(l1, l1));
  EXPECT_EQ(true, SameLine(l1, l1_same));
  EXPECT_EQ(true, SameLine(l1, l1_same2));
  EXPECT_EQ(false, SameLine(l1, l1_parallel));
  EXPECT_EQ(false, SameLine(l1, l2));
  EXPECT_EQ(false, SameLine(l1, l3));
}

TEST(Reflection, Line) {
  Line l(Point(mpq_class(0, 1), mpq_class(1, 2)), Point(mpq_class(1, 1), mpq_class(1, 2)));
  Line s(Point(mpq_class(1, 2), mpq_class(0, 1)), Point(mpq_class(1, 2), mpq_class(1, 3)));
  Line ref = reflection(l, s);
  EXPECT_EQ(Line(Point(mpq_class(1, 2), mpq_class(1, 1)), Point(mpq_class(1, 2), mpq_class(2, 3))), ref);
}

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
