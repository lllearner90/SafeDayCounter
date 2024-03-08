#include "../conv.h"
#include <gtest/gtest.h>

TEST(FloatToString, Resolution) {
    char  buf[5] = "0000";
    float count  = ((float) 158) / 365;
    floatToChar(count, buf, 2, sizeof(buf));
    EXPECT_STREQ("0.43", buf);
}