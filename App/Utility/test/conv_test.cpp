#include "../conv.h"
#include <gtest/gtest.h>

TEST(FloatToString, Resolution) {
    char buf[5] = "";
    floatToChar(3.14, buf, 2, sizeof(buf));
    EXPECT_STREQ("3.14", buf);
}