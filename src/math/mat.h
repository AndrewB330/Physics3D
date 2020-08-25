#pragma once
#include "vec3.h"
using T = double;
struct Mat33 {
    T val[3][3] = { {0,0,0},{0,0,0},{0,0,0} };
    Mat33();

    const T* operator[](size_t i) const;
    T* operator[](size_t i);

    Mat33 Inverse() const;
};

Mat33 operator*(const Mat33& lhs, const Mat33& rhs);

Vec3 operator*(const Mat33& lhs, const Vec3& rhs);

Mat33 operator*(const Mat33& lhs, T rhs);

Mat33 operator*(T lhs, const Mat33& rhs);