#pragma once
#include "vec3.hpp"
using T = double;
struct Mat3 {
    T val[3][3] = { {0,0,0},{0,0,0},{0,0,0} };
    Mat3();

    const T* operator[](size_t i) const;
    T* operator[](size_t i);

    Mat3 Inverse() const;

    static Mat3 Identity();

    Mat3 T() const;
};

Mat3 operator*(const Mat3& lhs, const Mat3& rhs);

Vec3 operator*(const Mat3& lhs, const Vec3& rhs);

Mat3 operator*(const Mat3& lhs, T rhs);

Mat3 operator*(T lhs, const Mat3& rhs);