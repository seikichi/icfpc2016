#include "gtest/gtest.h"
#include "in.h"

#include <string>

TEST(WriteInput, ShouldSameInput) {
  Input input1, input2;
  input1.ReadInput("./test/files/50.in");
  input1.WriteInput("./test/files/50.in.same");
  input2.ReadInput("./test/files/50.in.same");
  EXPECT_EQ(input2.silhouettes, input1.silhouettes);
  EXPECT_EQ(input2.skeltons, input1.skeltons);
  remove("./test/files/50.in.same");
}

TEST(Validate, Check) {
  Input input;
  input.ReadInput("./test/files/50.in");
  std::string str = input.WriteString();
  EXPECT_EQ(true, input.Validate(str));
  str += std::string(5000, ' ');
  EXPECT_EQ(false, input.Validate(str));
}
