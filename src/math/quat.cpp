#include "quat.h"
#include <cmath>

Quat::Quat() :s(1.0), v(Vec3()) {}

Quat::Quat(T s, Vec3 v) : s(s), v(v) {}

Quat& Quat::operator*=(const Quat& other) {
    double snew = s * other.s - Dot(v, other.v);
    Vec3 vnew = s * other.v + other.s * v + Cross(v, other.v);
    s = snew;
    v = vnew;
    return *this;
}

Quat& Quat::operator+=(const Quat& other) {
    s += other.s;
    v += other.v;
    return *this;
}

Quat Quat::Norm() const {
    double norm = sqrt(s * s + v.LenSqr());
    return Quat(s / norm, v / norm);
}

Mat33 Quat::Mat() const {
    Mat33 m;
    m[0][0] = 1 - 2 * v.y * v.y - 2 * v.z * v.z;
    m[0][1] = 2 * v.x * v.y - 2 * s * v.z;
    m[0][2] = 2 * v.x * v.z + 2 * s * v.y;

    m[1][0] = 2 * v.x * v.y + 2 * s * v.z;
    m[1][1] = 1 - 2 * v.x * v.x - 2 * v.z * v.z;
    m[1][2] = 2 * v.y * v.z - 2 * s * v.x;

    m[2][0] = 2 * v.x * v.z - 2 * s * v.y;
    m[2][1] = 2 * v.y * v.z + 2 * s * v.x;
    m[2][2] = 1 - 2 * v.x * v.x - 2 * v.y * v.y;
    return m;
}

Quat operator+(const Quat& lhs, const Quat& rhs) {
    return Quat(lhs) += rhs;
}

Quat operator*(const Quat& lhs, const Quat& rhs) {
    return Quat(lhs) *= rhs;
}

Quat operator*(const Vec3& lhs, const Quat& rhs) {
    return Quat(0, lhs) *= rhs;
}

Quat operator*(const Quat& lhs, T rhs) {
    return Quat(lhs.s * rhs, lhs.v * rhs);
}

Quat operator*(T lhs, const Quat& rhs) {
    return rhs * lhs;
}
