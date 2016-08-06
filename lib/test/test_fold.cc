#include "gtest/gtest.h"
#include "fold.h"
#include "xor_shift.h"

#include <iostream>
using namespace std;

TEST(Fold, AxisParallel) {
  //  ------
  //  |    | <= rest 
  //  |    |
  //  -----> 
  //  |    | <= discard
  //  ------
  Output output;
  output.Init();

  Line l(
      Point(mpq_class(0, 1), mpq_class(1, 3)),
      Point(mpq_class(1, 1), mpq_class(1, 3)));

  Output next = Fold(output, l);

  EXPECT_EQ(true, next.Validate());
  EXPECT_EQ(6, next.source_points.size());
  EXPECT_EQ(6, next.dest_points.size());
  EXPECT_EQ(2, next.facet_indecies.size());
  EXPECT_EQ(4, next.facet_indecies[0].size());
  EXPECT_EQ(4, next.facet_indecies[1].size());

  EXPECT_EQ(Point(mpq_class(0, 1), mpq_class(0, 1)), next.source_points[0]);
  EXPECT_EQ(Point(mpq_class(1, 1), mpq_class(0, 1)), next.source_points[1]);
  EXPECT_EQ(Point(mpq_class(1, 1), mpq_class(1, 1)), next.source_points[2]);
  EXPECT_EQ(Point(mpq_class(0, 1), mpq_class(1, 1)), next.source_points[3]);
  EXPECT_EQ(Point(mpq_class(1, 1), mpq_class(1, 3)), next.source_points[4]);
  EXPECT_EQ(Point(mpq_class(0, 1), mpq_class(1, 3)), next.source_points[5]);

  EXPECT_EQ(Point(mpq_class(0, 1), mpq_class(2, 3)), next.dest_points[0]);
  EXPECT_EQ(Point(mpq_class(1, 1), mpq_class(2, 3)), next.dest_points[1]);
  EXPECT_EQ(Point(mpq_class(1, 1), mpq_class(1, 1)), next.dest_points[2]);
  EXPECT_EQ(Point(mpq_class(0, 1), mpq_class(1, 1)), next.dest_points[3]);
  EXPECT_EQ(Point(mpq_class(1, 1), mpq_class(1, 3)), next.dest_points[4]);
  EXPECT_EQ(Point(mpq_class(0, 1), mpq_class(1, 3)), next.dest_points[5]);

  EXPECT_EQ(4, next.facet_indecies[0][0]);
  EXPECT_EQ(2, next.facet_indecies[0][1]);
  EXPECT_EQ(3, next.facet_indecies[0][2]);
  EXPECT_EQ(5, next.facet_indecies[0][3]);
  EXPECT_EQ(5, next.facet_indecies[1][0]);
  EXPECT_EQ(4, next.facet_indecies[1][1]);
  EXPECT_EQ(1, next.facet_indecies[1][2]);
  EXPECT_EQ(0, next.facet_indecies[1][3]);
}

TEST(Fold, Foldsome) {
  Output output1;
  output1.Init();

  Line l1(
      Point(mpq_class(0, 8), mpq_class(0, 1)),
      Point(mpq_class(1, 8), mpq_class(1, 1)));
  output1 = Fold(output1, l1);
  // output1.WriteOutput("hoge.out");

  Line l2(
      Point(mpq_class(1, 1), mpq_class(0, 1)),
      Point(mpq_class(0, 1), mpq_class(1, 1)));
  output1 = Fold(output1, l2);
  // output1.WriteOutput("hoge.out");

  Line l3(
      Point(mpq_class(0, 2), mpq_class(1, 2)),
      Point(mpq_class(1, 2), mpq_class(1, 2)));
  output1 = Fold(output1, l3);
  // output1.WriteOutput("hoge.out");
  // output1.WriteSVGDest("hoge.svg");
  // system("firefox hoge.svg");

  Output output2;
  output2.ReadOutput("./test/files/cut1.out");

  EXPECT_EQ(true, output1.Validate());
}

TEST(Fold, SameLine) {
  Output output1;
  output1.Init();

  Line l(
      Point(mpq_class(0, 1), mpq_class(1, 3)),
      Point(mpq_class(1, 1), mpq_class(2, 3)));
  output1 = Fold(output1, l);
  output1 = Fold(output1, l);
  output1 = Fold(output1, l);
  output1 = Fold(output1, l);
  output1 = Fold(output1, l);

  Output output2;
  output2.ReadOutput("./test/files/cut1.out");

  EXPECT_EQ(output2.source_points, output1.source_points);
  EXPECT_EQ(output2.facet_indecies, output1.facet_indecies);
  EXPECT_EQ(output2.dest_points, output1.dest_points);
}

TEST(Fold, Fold2) {
  Output output1;
  output1.Init();

  Line l1(
      Point(mpq_class(0, 1), mpq_class(1, 3)),
      Point(mpq_class(1, 1), mpq_class(2, 3)));
  output1 = Fold(output1, l1);

  Line l2(
      Point(mpq_class(1, 3), mpq_class(0, 1)),
      Point(mpq_class(2, 3), mpq_class(1, 1)));
  output1 = Fold(output1, l2);

  Output output2;
  output2.ReadOutput("./test/files/cut2.out");

  EXPECT_EQ(true, output1.Validate());
  EXPECT_EQ(output2.source_points, output1.source_points);
  EXPECT_EQ(output2.facet_indecies, output1.facet_indecies);
  EXPECT_EQ(output2.dest_points, output1.dest_points);
}

TEST(Fold, FoldMany) {
  Output output1;
  output1.Init();

  Random rnd(12345);
  for (int i = 0; i < 7; i++) {
    int prev_cnt = output1.source_points.size();
    Line l1(
        Point(mpq_class(rnd.next(2, 28), 30), mpq_class(rnd.next(2, 28), 30)),
        Point(mpq_class(rnd.next(2, 28), 30), mpq_class(rnd.next(2, 28), 30)));
    output1 = Fold(output1, l1);
    if (prev_cnt == (int)output1.source_points.size()) {
      i--; continue;
    }
    // char buffer[100];
    // snprintf(buffer, 99, "cut%d.out", i + 1);
    // output1.WriteOutput(buffer);
    // snprintf(buffer, 99, "cut%d_source.svg", i + 1);
    // output1.WriteSVGSource(buffer);
    // snprintf(buffer, 99, "cut%d_dest.svg", i + 1);
    // output1.WriteSVGDest(buffer);
  }

  Output output2;
  output2.ReadOutput("./test/files/cut7.out");

  EXPECT_EQ(output2.source_points, output1.source_points);
  EXPECT_EQ(output2.facet_indecies, output1.facet_indecies);
  EXPECT_EQ(output2.dest_points, output1.dest_points);
}
