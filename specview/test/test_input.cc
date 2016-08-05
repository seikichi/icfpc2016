#include "gtest/gtest.h"
#include "input.h"

TEST(ReadFraction, WithSlash) {
  EXPECT_EQ(mpq_class(11, 345), ReadFraction("11/345"));
}

TEST(ReadFraction, WithoutSlash) {
  EXPECT_EQ(mpq_class(123, 1), ReadFraction("123"));
}

TEST(ReadPoint, Normal) {
  Point p = ReadPoint("3/14,5/11");
  EXPECT_EQ(mpq_class(3, 14), p.x);
  EXPECT_EQ(mpq_class(5, 11), p.y);
}
