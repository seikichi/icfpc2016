#include "gtest/gtest.h"
#include "out.h"
#include <iostream>

TEST(StrFraction, WithSlash) {
  EXPECT_EQ(mpq_class(2,3).get_str(), "2/3");
}

TEST(StrFraction, WithoutSlash) {
  EXPECT_EQ(mpq_class(2,1).get_str(), "2");
}

TEST(Validation, SmallFacet) {
  Output output;
  output.Init();
  output.facet_indecies.push_back({0, 1});
  EXPECT_EQ(false, output.Validate());
}

TEST(WriteOutput, ShouldSameOutput) {
  Output output1, output2;
  output1.ReadOutput("./test/files/sample.out");
  output1.WriteOutput("./test/files/sample.out.same");
  output2.ReadOutput("./test/files/sample.out.same");
  EXPECT_EQ(output2.source_points, output1.source_points);
  EXPECT_EQ(output2.facet_indecies, output1.facet_indecies);
  EXPECT_EQ(output2.dest_points, output1.dest_points);
  remove("./test/files/sample.out.same");
}

TEST(Validate, Check) {
  Output output;
  output.ReadOutput("./test/files/sample.out");
  std::string str = output.WriteString();
  EXPECT_EQ(true, output.Validate());
}

TEST(ReadOuput, Canonicalize) {
  Output output;
  output.ReadOutput("./test/files/cut7.out");
  Point p = output.source_points[4];
  canonicalize(p);
  EXPECT_EQ(p, output.source_points[4]);
}

TEST(WriteOutput, Hoge) {
  mpq_class v(2,4);
  v.canonicalize();
  EXPECT_EQ("1/2", v.get_str());

  Output output;
  output.Init();
  output.dest_points[3] = Point(mpq_class(0, 10000), mpq_class(333, 333));
  std::string str = output.WriteString();
  EXPECT_EQ(nullptr, strstr(str.c_str(), "333"));
}
