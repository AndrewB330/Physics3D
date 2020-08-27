#include <gtest/gtest.h>
#include <math/vec3.hpp>

TEST(Vec3, operators) {
    EXPECT_DOUBLE_EQ((Vec3(1, 2, 1) + Vec3(-0.5, 0.23, 0.5)).x , 0.5);
    EXPECT_DOUBLE_EQ((Vec3(1, 2, 1) + Vec3(-0.5, 0.23, 0.5)).y , 2.23);
    EXPECT_DOUBLE_EQ((Vec3(1, 2, 1) + Vec3(-0.5, 0.23, 0.5)).z , 1.5);
}
