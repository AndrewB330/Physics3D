#include <gtest/gtest.h>
#include <engine/geometry/shape.hpp>

TEST(IsPointInside, Sphere) {
    auto s1 = std::make_unique<SphereShape>(1.0);
    s1->Translate(Vec3(1, 0, 0));

    EXPECT_TRUE(s1->IsPointInside(Vec3(1, 0, 0)));
    EXPECT_TRUE(s1->IsPointInside(Vec3(1.999, 0, 0)));
    EXPECT_TRUE(s1->IsPointInside(Vec3(1, 0.999, 0)));

    EXPECT_FALSE(s1->IsPointInside(Vec3(1, 1.001, 0)));
    EXPECT_FALSE(s1->IsPointInside(Vec3(2.001, 0, 0)));
}

TEST(IsPointInside, Box) {
    auto s1 = std::make_unique<BoxShape>(2.0);

    EXPECT_TRUE(s1->IsPointInside(Vec3(0.999, 0, 0)));
    EXPECT_TRUE(s1->IsPointInside(Vec3(0.999, -0.999, 0.999)));
    EXPECT_TRUE(s1->IsPointInside(Vec3(0.5, 0.999, 0.999)));

    EXPECT_FALSE(s1->IsPointInside(Vec3(0.5, 0.999, 1.001)));
    EXPECT_FALSE(s1->IsPointInside(Vec3(0, 0, -1.001)));
}