#pragma once

const double PI = 3.14159265358979323846;

using T = double;

struct Vec3 {
    T x, y, z;
    Vec3(T x, T y, T z);
    Vec3();

    T LenSqr() const;

    T Len() const;

    Vec3& operator+=(const Vec3& other);

    Vec3& operator-=(const Vec3& other);

    Vec3& operator*=(T mul);

    Vec3& operator/=(T div);

    Vec3 Norm();
};

Vec3 operator+(const Vec3& lhs, const Vec3& rhs);

Vec3 operator-(const Vec3& lhs, const Vec3& rhs);

Vec3 operator*(const Vec3& lhs, T rhs);

Vec3 operator*(T lhs, const Vec3& rhs);

Vec3 operator/(const Vec3& lhs, T rhs);

Vec3 operator-(const Vec3& v);

T DistSqr(const Vec3& lhs, const Vec3& rhs);

T Dist(const Vec3& lhs, const Vec3& rhs);

T Dot(const Vec3& lhs, const Vec3& rhs);

Vec3 Cross(const Vec3& a, const Vec3& b);
