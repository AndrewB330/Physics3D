#include <gtest/gtest.h>
#include <common/math/vec3.hpp>
#include <common/math/quat.hpp>

TEST(Vec3, operators) {
    EXPECT_DOUBLE_EQ((Vec3(1, 2, 1) + Vec3(-0.5, 0.23, 0.5)).x , 0.5);
    EXPECT_DOUBLE_EQ((Vec3(1, 2, 1) + Vec3(-0.5, 0.23, 0.5)).y , 2.23);
    EXPECT_DOUBLE_EQ((Vec3(1, 2, 1) + Vec3(-0.5, 0.23, 0.5)).z , 1.5);
}

TEST(Quat, rotations) {
    for(int i = 0; i < 10; i++) {
        Vec3 v = Vec3::RandomUnit() * 10;
        Quat q = Quat::Rotation(0.8, Vec3(5, -1, -2));
        Mat3 mat = q.Mat();
        Mat3 mat_inv = mat.T();

        Vec3 q_v = q.ApplyRotation(v);
        Vec3 mat_v = mat * v;
        Vec3 mat_v_inv = mat_inv * v;
        Vec3 q_v_inv = q.ApplyInvRotation(v);

        EXPECT_NEAR(q_v.x, mat_v.x, 1e-6);
        EXPECT_NEAR(q_v.y, mat_v.y, 1e-6);
        EXPECT_NEAR(q_v.z, mat_v.z, 1e-6);
        EXPECT_NEAR(q_v_inv.x, mat_v_inv.x, 1e-6);
        EXPECT_NEAR(q_v_inv.y, mat_v_inv.y, 1e-6);
        EXPECT_NEAR(q_v_inv.z, mat_v_inv.z, 1e-6);
    }
}
