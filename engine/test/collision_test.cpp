#include <gtest/gtest.h>
#include <engine/model/objects/sphere.hpp>
#include <engine/model/objects/box.hpp>
#include <engine/collision.hpp>

TEST(Collision, SphereToSphere) {
    Sphere *s1 = new Sphere(1.0);
    Sphere *s2 = new Sphere(1.0);

    s2->SetPosition(Vec3(1.190, -1.0619, 1.20669));
    CollisionDetector detector_0(s1, s2);

    s2->SetPosition(Vec3(1.191, -1.0619, 1.20669));
    CollisionDetector detector_1(s1, s2);

    EXPECT_TRUE(detector_0.HasCollision());
    EXPECT_FALSE(detector_1.HasCollision());
}

TEST(Collision, SphereToBox) {
    Sphere *s = new Sphere(1.0);
    Box *b = new Box(1.0, 2.0, 1.5);

    s->SetPosition(Vec3(0.1, -0.2, 0.33));
    b->SetPosition(Vec3(0.9652, -1.13, -1.628));
    b->SetRotation(Quat::Rotation(0.410, Vec3(1, 0.5, 0.8)));
    CollisionDetector detector_0(s, b);

    b->SetPosition(Vec3(0.9652, -1.13, -1.632));
    CollisionDetector detector_1(s, b);

    EXPECT_TRUE(detector_0.HasCollision());
    EXPECT_FALSE(detector_1.HasCollision());
}

TEST(Collision, BoxToBoxAxisAligned) {
    Box *b1 = new Box(1.0, 1.0, 1.0);
    Box *b2 = new Box(1.0, 1.0, 1.0);

    b2->SetPosition(Vec3(0.99999, 0.0, 0.0));
    CollisionDetector detector_0(b1, b2);
    b2->SetPosition(Vec3(1.00001, 0.0, 0.0));
    CollisionDetector detector_1(b1, b2);
    b2->SetPosition(Vec3(0.99999, 0.99999, 0.999999));
    CollisionDetector detector_2(b1, b2);

    EXPECT_TRUE(detector_0.HasCollision());
    EXPECT_FALSE(detector_1.HasCollision());
    EXPECT_TRUE(detector_2.HasCollision());
}

TEST(Collision, BoxToBoxEdgeToEdge) {
    Box *b1 = new Box(1.0, 1.0, 1.0);
    Box *b2 = new Box(1.0, 1.0, 1.0);

    b2->SetRotation(Quat::Rotation(PI / 2, Vec3(1, 0, 1)));
    b2->SetPosition(Vec3(1.0, 0.0, 1.0) * (1 - 1e-7));
    CollisionDetector detector_0(b1, b2);

    b2->SetPosition(Vec3(1.0, 0.0, 1.0) * (1 + 1e-7));
    CollisionDetector detector_1(b1, b2);

    EXPECT_TRUE(detector_0.HasCollision());
    EXPECT_FALSE(detector_1.HasCollision());
}

TEST(Collision, BoxToBoxVertexToFace) {
    Box *b1 = new Box(1.0, 1.0, 1.0);
    Box *b2 = new Box(1.0, 1.0, 1.0);

    b2->SetRotation(Quat::Rotation(PI / 2, Vec3(1, 0, 1)));
    b2->SetPosition(Vec3(1.0, 0.0, 0.0) * (sqrt(2) / 4 + 1.0 - 1e-7));
    CollisionDetector detector_0(b1, b2);

    b2->SetPosition(Vec3(1.0, 0.0, 0.0) * (sqrt(2) / 4 + 1.0 + 1e-7));
    CollisionDetector detector_1(b1, b2);

    EXPECT_TRUE(detector_0.HasCollision());
    EXPECT_FALSE(detector_1.HasCollision());
}

