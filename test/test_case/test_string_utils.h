#include <gtest/gtest.h>

#include "string_utils.h"

class StringUtilsTest : public testing::Test {
};

TEST_F(StringUtilsTest, isDigitTest) {
    ASSERT_TRUE(isDigit("0"));
    ASSERT_TRUE(isDigit("1"));
    ASSERT_FALSE(isDigit(""));
    ASSERT_FALSE(isDigit("a"));
    ASSERT_FALSE(isDigit("a1"));
}