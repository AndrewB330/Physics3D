#pragma once
#include "vec3.hpp"
#include "mat3.hpp"
#include <math/rand.hpp>

using T = double;

struct Quat {
    T s;
    Vec3 v;

    Quat();
    Quat(T s, Vec3 v);

    Quat& operator*=(const Quat& other);
    Quat& operator+=(const Quat& other);

    Quat Norm() const;

    Mat3 Mat() const;

    static Quat Identity();

    static Quat RandomRotation();

    static Quat Rotation(double angle, Vec3 axis);

private:
    static RandomGen rand;
};

Quat operator+(const Quat& lhs, const Quat& rhs);
Quat operator*(const Quat& lhs, const Quat& rhs);
Quat operator*(const Vec3& lhs, const Quat& rhs);
Quat operator*(const Quat& lhs, T rhs);
Quat operator*(T lhs, const Quat& rhs);