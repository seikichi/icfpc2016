#include "gtest/gtest.h"
#include "in.h"

TEST(WriteInput, ShouldSameInput) {
  Input input1, input2;
  input1.ReadInput("./test/files/50.in");
  input1.WriteInput("./test/files/50.in.same");
  input2.ReadInput("./test/files/50.in.same");
  EXPECT_EQ(input1.silhouettes, input2.silhouettes);
  EXPECT_EQ(input1.skeltons, input2.skeltons);
  remove("./test/files/50.in.same");
}
