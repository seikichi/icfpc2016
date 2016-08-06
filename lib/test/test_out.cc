#include "gtest/gtest.h"
#include "out.h"

TEST(StrFraction, WithSlash) {
  EXPECT_EQ(mpq_class(2,3).get_str(), "2/3");
}

TEST(StrFraction, WithoutSlash) {
  EXPECT_EQ(mpq_class(2,1).get_str(), "2");
}

TEST(WriteOutput, ShouldSameOutput) {
  Output output1, output2;
  output1.ReadOutput("./test/files/sample.out");
  output1.WriteOutput("./test/files/sample.out.same");
  output2.ReadOutput("./test/files/sample.out.same");
  EXPECT_EQ(output1.source_points, output2.source_points);
  EXPECT_EQ(output1.facet_indecies, output2.facet_indecies);
  EXPECT_EQ(output1.dest_points, output2.dest_points);
  remove("./test/files/sample.out.same");
}

TEST(Validate, Check) {
  Output output;
  output.ReadOutput("./test/files/sample.out");
  std::string str = output.WriteString();
  EXPECT_EQ(output.Validate(str), true);
  str += std::string(5000, ' ');
  EXPECT_EQ(output.Validate(str), false);
}
