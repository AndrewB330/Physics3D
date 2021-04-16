#include <math/vec3i.hpp>

using S = int64_t;

Vec3i::Vec3i(S x, S y, S z) : x(x), y(y), z(z) {}

Vec3i::Vec3i() : x(S()), y(S()), z(S()) {}

Vec3i &Vec3i::operator+=(const Vec3i &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vec3i &Vec3i::operator-=(const Vec3i &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vec3i &Vec3i::operator*=(S mul) {
    x *= mul;
    y *= mul;
    z *= mul;
    return *this;
}


Vec3i &Vec3i::operator/=(S div) {
    x /= div;
    y /= div;
    z /= div;
    return *this;
}

std::tuple<S, S, S> Vec3i::ToTuple() const {
    return {x, y, z};
}

bool operator<(const Vec3i & lhs, const Vec3i &rhs) {
    return (lhs.x == rhs.x ? (lhs.y == rhs.y ? lhs.z < rhs.z : lhs.y < rhs.y) : lhs.x < rhs.x);
}

Vec3i operator+(const Vec3i &lhs, const Vec3i &rhs) {
    return Vec3i(lhs) += rhs;
}

Vec3i operator-(const Vec3i &lhs, const Vec3i &rhs) {
    return Vec3i(lhs) -= rhs;
}

Vec3i operator*(const Vec3i &lhs, S rhs) {
    return Vec3i(lhs) *= rhs;
}

Vec3i operator*(S lhs, const Vec3i &rhs) {
    return Vec3i(rhs) *= lhs;
}

Vec3i operator*(const Vec3i &lhs, const Vec3i &rhs) {
    return Vec3i(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

Vec3i operator/(const Vec3i &lhs, S rhs) {
    return Vec3i(lhs) /= rhs;
}

Vec3i operator-(const Vec3i &v) {
    return Vec3i(-v.x, -v.y, -v.z);
}

std::ostream &operator<<(std::ostream &out, const Vec3i &v) {
    return out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}