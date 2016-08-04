#include "answer.h"
#include "gtest/gtest.h"

TEST(AnswerTest, UltimateQuestion) {
    EXPECT_EQ(42, Answer().ultimate_question());
}
