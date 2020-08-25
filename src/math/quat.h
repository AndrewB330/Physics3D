#pragma once
#include "vec3.h"
#include "mat.h"

using T = double;

struct Quat {
    T s;
    Vec3 v;

    Quat();
    Quat(T s, Vec3 v);

    Quat& operator*=(const Quat& other);
    Quat& operator+=(const Quat& other);

    Quat Norm() const;

    Mat33 Mat() const;
};

Quat operator+(const Quat& lhs, const Quat& rhs);
Quat operator*(const Quat& lhs, const Quat& rhs);
Quat operator*(const Vec3& lhs, const Quat& rhs);
Quat operator*(const Quat& lhs, T rhs);
Quat operator*(T lhs, const Quat& rhs);