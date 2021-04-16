#include <math/vec3.hpp>
#include <cmath>

using T = double;

Vec3::Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

Vec3::Vec3() : x(T()), y(T()), z(T()) {}

RandomGen Vec3::rand = RandomGen(0);

T Vec3::LenSqr() const {
    return x * x + y * y + z * z;
}

T Vec3::Len() const {
    return sqrt(LenSqr());
}

Vec3 &Vec3::operator+=(const Vec3 &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vec3 &Vec3::operator-=(const Vec3 &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vec3 &Vec3::operator*=(T mul) {
    x *= mul;
    y *= mul;
    z *= mul;
    return *this;
}


Vec3 &Vec3::operator/=(T div) {
    T inv = T(1.0) / div;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
}

Vec3 Vec3::Norm() const {
    return Vec3(*this) / Len();
}

Vec3 Vec3::RandomUnit() {
    double x = rand.get_double(-1, 1);
    double y = rand.get_double(-1, 1);
    double z = rand.get_double(-1, 1);
    if (x * x + y * y + z * z < 1e-3) {
        return RandomUnit();
    }
    return Vec3(x, y, z).Norm();
}

bool Vec3::operator<(const Vec3 &other) const {
    return (x != other.x ? x < other.x : y < other.y);
}

Vec3 operator+(const Vec3 &lhs, const Vec3 &rhs) {
    return Vec3(lhs) += rhs;
}

Vec3 operator-(const Vec3 &lhs, const Vec3 &rhs) {
    return Vec3(lhs) -= rhs;
}

Vec3 operator*(const Vec3 &lhs, T rhs) {
    return Vec3(lhs) *= rhs;
}

Vec3 operator*(T lhs, const Vec3 &rhs) {
    return Vec3(rhs) *= lhs;
}

Vec3 operator*(const Vec3 &lhs, const Vec3 &rhs) {
    return Vec3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

Vec3 operator/(const Vec3 &lhs, T rhs) {
    return Vec3(lhs) /= rhs;
}

Vec3 operator-(const Vec3 &v) {
    return Vec3(-v.x, -v.y, -v.z);
}

T DistSqr(const Vec3 &lhs, const Vec3 &rhs) {
    return (lhs - rhs).LenSqr();
}

T Dist(const Vec3 &lhs, const Vec3 &rhs) {
    return (lhs - rhs).Len();
}

T Dot(const Vec3 &lhs, const Vec3 &rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

Vec3 Cross(const Vec3 &a, const Vec3 &b) {
    return Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

std::ostream &operator<<(std::ostream &out, const Vec3 &v) {
    return out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

Vec3 Sign(const Vec3 &vec) {
    return Vec3(
            (vec.x < 0) ? -1 : (vec.x > 0 ? 1 : 1),
            (vec.y < 0) ? -1 : (vec.y > 0 ? 1 : 1),
            (vec.z < 0) ? -1 : (vec.z > 0 ? 1 : 1)
    );
}