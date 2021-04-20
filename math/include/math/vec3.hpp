#pragma once

#include <iostream>
#include <common/random.hpp>

const double PI = 3.14159265358979323846;

using T = double;

struct Vec3 {
    T x, y, z;

    Vec3(T x, T y, T z);

    Vec3();

    T LenSqr() const;

    T Len() const;

    Vec3 &operator+=(const Vec3 &other);

    Vec3 &operator-=(const Vec3 &other);

    Vec3 &operator*=(T mul);

    Vec3 &operator/=(T div);

    [[nodiscard]] Vec3 Norm() const;

    static Vec3 RandomUnit();

    bool operator<(const Vec3 &other) const;

    static Vec3 Min(const Vec3 & lhs, const Vec3& rhs);

    static Vec3 Max(const Vec3 & lhs, const Vec3& rhs);

private:
    static RandomGen rand;
};

std::ostream &operator<<(std::ostream &out, const Vec3 &v);

Vec3 operator+(const Vec3 &lhs, const Vec3 &rhs);

Vec3 operator-(const Vec3 &lhs, const Vec3 &rhs);

Vec3 operator*(const Vec3 &lhs, T rhs);

Vec3 operator*(const Vec3 &lhs, const Vec3& rhs);

Vec3 operator*(T lhs, const Vec3 &rhs);

Vec3 operator/(const Vec3 &lhs, T rhs);

Vec3 operator-(const Vec3 &v);

T DistSqr(const Vec3 &lhs, const Vec3 &rhs);

T Dist(const Vec3 &lhs, const Vec3 &rhs);

T Dot(const Vec3 &lhs, const Vec3 &rhs);

Vec3 Cross(const Vec3 &a, const Vec3 &b);

Vec3 Sign(const Vec3& vec);
