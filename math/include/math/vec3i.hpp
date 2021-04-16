#pragma once

#include <iostream>
#include <tuple>

using S = int64_t;

struct Vec3i {
    S x, y, z;

    Vec3i(S x, S y, S z);

    Vec3i();

    Vec3i &operator+=(const Vec3i &other);

    Vec3i &operator-=(const Vec3i &other);

    Vec3i &operator*=(S mul);

    Vec3i &operator/=(S div);

    std::tuple<S, S, S> ToTuple() const;
};

std::ostream &operator<<(std::ostream &out, const Vec3i &v);

bool operator<(const Vec3i & lhs, const Vec3i &rhs);

Vec3i operator+(const Vec3i &lhs, const Vec3i &rhs);

Vec3i operator-(const Vec3i &lhs, const Vec3i &rhs);

Vec3i operator*(const Vec3i &lhs, S rhs);

Vec3i operator*(S lhs, const Vec3i &rhs);

Vec3i operator/(const Vec3i &lhs, S rhs);

Vec3i operator-(const Vec3i &v);
