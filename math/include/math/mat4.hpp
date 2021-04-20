#pragma once

#include "quat.hpp"

using T = double;

struct Mat4 {
    T val[4][4] = {{0, 0, 0, 0},
                   {0, 0, 0, 0},
                   {0, 0, 0, 0},
                   {0, 0, 0, 0}};
    Mat4() = default;

    static Mat4 Transformation(Vec3 translation, Quat rotation, double scale = 1.0);

    static Mat4 TransformationInv(Vec3 translation, Quat rotation, double scale = 1.0);
};

Mat4 operator*(const Mat4 & lhs, T rhs);