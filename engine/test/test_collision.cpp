#include <gtest/gtest.h>
#include <engine/collision/collision.hpp>

const double DISTANCE_EPSILON = 1e-7;

const double DIRECTION_EPSILON = 1e-7;
const double WEAK_DIRECTION_EPSILON = 1e-4; // for curved shapes (e.g. sphere)

TEST(GJK, SphereToSphere) {
    auto s1 = std::make_unique<SphereShape>(1.0);
    auto s2 = std::make_unique<SphereShape>(1.0);

    s2->SetTranslation(Vec3(1.190, -1.0619, 1.20669));
    GilbertJohnsonKeerthi gjk_0(s1.get(), s2.get());

    s2->SetTranslation(Vec3(1.191, -1.0619, 1.20669));
    GilbertJohnsonKeerthi gjk_1(s1.get(), s2.get());

    EXPECT_TRUE(gjk_0.ContainsOrigin());
    EXPECT_FALSE(gjk_1.ContainsOrigin());
}

TEST(GJK, SphereToBox) {
    auto s = std::make_unique<SphereShape>(1.0);
    auto b = std::make_unique<BoxShape>(1.0, 2.0, 1.5);

    s->SetTranslation(Vec3(0.1, -0.2, 0.33));
    b->SetTranslation(Vec3(0.9652, -1.13, -1.628));
    b->SetRotation(Quat::Rotation(0.410, Vec3(1, 0.5, 0.8)));
    GilbertJohnsonKeerthi gjk_0(s.get(), b.get());

    b->SetTranslation(Vec3(0.9652, -1.13, -1.632));
    GilbertJohnsonKeerthi gjk_1(s.get(), b.get());

    EXPECT_TRUE(gjk_0.ContainsOrigin());
    EXPECT_FALSE(gjk_1.ContainsOrigin());
}

TEST(GJK, BoxToBoxAxisAligned) {
    auto b1 = std::make_unique<BoxShape>(1.0);
    auto b2 = std::make_unique<BoxShape>(1.0);

    b2->SetTranslation(Vec3(0.99999, 0.0, 0.0));
    GilbertJohnsonKeerthi gjk_0(b1.get(), b2.get());

    b2->SetTranslation(Vec3(1.00001, 0.0, 0.0));
    GilbertJohnsonKeerthi gjk_1(b1.get(), b2.get());

    b2->SetTranslation(Vec3(0.99999, 0.99999, 0.999999));
    GilbertJohnsonKeerthi gjk_2(b1.get(), b2.get());

    EXPECT_TRUE(gjk_0.ContainsOrigin());
    EXPECT_FALSE(gjk_1.ContainsOrigin());
    EXPECT_TRUE(gjk_2.ContainsOrigin());
}

TEST(GJK, BoxToBoxEdgeToEdge) {
    auto b1 = std::make_unique<BoxShape>(1.0);
    auto b2 = std::make_unique<BoxShape>(1.0);

    b2->SetRotation(Quat::Rotation(PI / 2, Vec3(1, 0, 1)));
    b2->SetTranslation(Vec3(1.0, 0.0, 1.0) * (1 - 1e-7));
    GilbertJohnsonKeerthi gjk_0(b1.get(), b2.get());

    b2->SetTranslation(Vec3(1.0, 0.0, 1.0) * (1 + 1e-7));
    GilbertJohnsonKeerthi gjk_1(b1.get(), b2.get());

    EXPECT_TRUE(gjk_0.ContainsOrigin());
    EXPECT_FALSE(gjk_1.ContainsOrigin());
}

TEST(GJK, BoxToBoxVertexToFace) {
    auto b1 = std::make_unique<BoxShape>(1.0);
    auto b2 = std::make_unique<BoxShape>(1.0);

    b2->SetRotation(Quat::Rotation(PI / 2, Vec3(1, 0, 1)));
    b2->SetTranslation(Vec3(1.0, 0.0, 0.0) * (sqrt(2) / 4 + 1.0 - 1e-7));
    GilbertJohnsonKeerthi gjk_0(b1.get(), b2.get());

    b2->SetTranslation(Vec3(1.0, 0.0, 0.0) * (sqrt(2) / 4 + 1.0 + 1e-7));
    GilbertJohnsonKeerthi gjk_1(b1.get(), b2.get());

    EXPECT_TRUE(gjk_0.ContainsOrigin());
    EXPECT_FALSE(gjk_1.ContainsOrigin());
}

TEST(EPA, Sphere) {
    auto s = std::make_unique<SphereShape>(1.0);
    Vec3 direction = Vec3(0.5, 0.3, 0.9).Norm();
    s->SetTranslation(direction * 0.8);
    GilbertJohnsonKeerthi gjk(s.get());

    ExpandingPolytopeAlgorithm epa(s.get(), gjk.GetSimplex());

    EXPECT_NEAR(epa.GetDistance(), 0.2, DISTANCE_EPSILON);
    EXPECT_LE((epa.GetDirection() + direction).Len(), WEAK_DIRECTION_EPSILON);
}

TEST(EPA, Box) {
    auto b = std::make_unique<BoxShape>(1.0);
    b->SetTranslation(Vec3(0.4, 0.1, 0.2));
    GilbertJohnsonKeerthi gjk(b.get());

    ExpandingPolytopeAlgorithm epa(b.get(), gjk.GetSimplex());

    EXPECT_NEAR(epa.GetDistance(), 0.1, DISTANCE_EPSILON);
    EXPECT_LE((epa.GetDirection() + Vec3(1, 0, 0)).Len(), DIRECTION_EPSILON);
}

TEST(EPA, BoxToBox) {
    auto b1 = std::make_unique<BoxShape>(1.0);
    auto b2 = std::make_unique<BoxShape>(1.0);
    b2->SetRotation(Quat::Rotation(PI / 2, Vec3(1, 0, 1)));
    b2->SetTranslation(Vec3(1.0, 0.0, 0.0) * (sqrt(2) / 4 + 1.0 - 0.05));
    b2->Translate(Vec3(0, 0.4, 0.43));
    auto md = std::make_unique<MinkowskiDifferenceShape>(b1.get(), b2.get());
    GilbertJohnsonKeerthi gjk(md.get());

    ExpandingPolytopeAlgorithm epa(md.get(), gjk.GetSimplex());

    EXPECT_NEAR(epa.GetDistance(), 0.05, DISTANCE_EPSILON);
    EXPECT_LE((epa.GetDirection() - Vec3(1, 0, 0)).Len(), DIRECTION_EPSILON);
}


TEST(EPA, SphereToBoxCorner) {
    auto b = std::make_unique<BoxShape>(1.0);
    auto s = std::make_unique<SphereShape>(1.0);
    double mul = (sqrt(3) * 0.5 + 1 - 0.05);
    s->Translate(Vec3(1, -1, 1).Norm() * mul);
    auto md = std::make_unique<MinkowskiDifferenceShape>(b.get(), s.get());
    GilbertJohnsonKeerthi gjk(md.get());

    EXPECT_TRUE(gjk.ContainsOrigin());

    ExpandingPolytopeAlgorithm epa(md.get(), gjk.GetSimplex());

    EXPECT_NEAR(epa.GetDistance(), 0.05, DISTANCE_EPSILON);
    EXPECT_LE((epa.GetDirection() - Vec3(1, -1, 1).Norm()).Len(), WEAK_DIRECTION_EPSILON);
}

TEST(EPA, SphereToBoxFace) {
    auto b = std::make_unique<BoxShape>(2.0);
    auto s = std::make_unique<SphereShape>(1.0);
    s->Translate(Vec3(2 - 0.01, 0.85, -0.35));
    auto md = std::make_unique<MinkowskiDifferenceShape>(b.get(), s.get());
    GilbertJohnsonKeerthi gjk(md.get());

    EXPECT_TRUE(gjk.ContainsOrigin());

    ExpandingPolytopeAlgorithm epa(md.get(), gjk.GetSimplex());

    EXPECT_NEAR(epa.GetDistance(), 0.01, DISTANCE_EPSILON);
    EXPECT_LE((epa.GetDirection() - Vec3(1, 0, 0)).Len(), WEAK_DIRECTION_EPSILON);
}

TEST(EPA, SphereToBoxEdge) {
    auto b = std::make_unique<BoxShape>(2.0);
    auto s = std::make_unique<SphereShape>(1.0);
    double mul = (sqrt(2) + 1 - 0.001);
    s->Translate(Vec3(1, 0, -1).Norm() * mul);
    auto md = std::make_unique<MinkowskiDifferenceShape>(b.get(), s.get());
    GilbertJohnsonKeerthi gjk(md.get());

    ExpandingPolytopeAlgorithm epa(md.get(), gjk.GetSimplex());
    EXPECT_NEAR(epa.GetDistance(), 0.001, DISTANCE_EPSILON);
    EXPECT_LE((epa.GetDirection() - Vec3(1, 0, -1).Norm()).Len(), WEAK_DIRECTION_EPSILON);
}

TEST(Collision, BoxToBoxEdgeToEdge) {
    auto b1 = std::make_unique<BoxShape>(1.0);
    auto b2 = std::make_unique<BoxShape>(1.0);

    b2->SetRotation(Quat::Rotation(PI / 2, Vec3(1, 0, 1)));
    b2->SetTranslation(Vec3(1.0, 0.0, 1.0) * (1 - 1e-3));

    CollisionDetector detector(b1.get(), b2.get());

    EXPECT_TRUE(detector.HasCollision());
    EXPECT_EQ(detector.GetCollisionPoints().size(), 1);
    EXPECT_NEAR(detector.GetCollisionPoints()[0].penetration, 1e-3 * sqrt(2), 1e-7);
}

TEST(Collision, BoxToBoxFaceToFace) {
    auto b1 = std::make_unique<BoxShape>(2.0);
    auto b2 = std::make_unique<BoxShape>(1.0);

    b2->SetTranslation(Vec3(1.49, 0.0, 0.0));

    CollisionDetector detector(b1.get(), b2.get());

    EXPECT_TRUE(detector.HasCollision());
    EXPECT_EQ(detector.GetCollisionPoints().size(), 4);
    EXPECT_NEAR(detector.GetCollisionPoints()[0].penetration, 1e-2, 1e-7);
    EXPECT_NEAR(detector.GetCollisionPoints()[1].penetration, 1e-2, 1e-7);
    EXPECT_NEAR(detector.GetCollisionPoints()[2].penetration, 1e-2, 1e-7);
    EXPECT_NEAR(detector.GetCollisionPoints()[3].penetration, 1e-2, 1e-7);
}
