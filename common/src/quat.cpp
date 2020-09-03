#include <common/math/quat.hpp>
#include <cmath>

Quat::Quat() :s(0.0), v(Vec3()) {}

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

Mat3 Quat::Mat() const {
    Mat3 m;
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

Quat Quat::Identity() {
    return Quat(1.0, Vec3());
}

Quat Quat::RandomRotation() {
    double angle = rand.get_double(-PI, PI);
    return Quat(cos(angle / 2), sin(angle / 2) * Vec3::RandomUnit());
}

Quat Quat::Rotation(double angle, Vec3 axis) {
    return Quat(cos(angle/2), axis.Norm() * sin(angle/2));
}

RandomGen Quat::rand = RandomGen(0);

Vec3 Quat::ApplyRotation(const Vec3& vec) const {
    Vec3 t = 2 * Cross(v, vec);
    return vec + s * t + Cross(v, t);
}

Vec3 Quat::ApplyInvRotation(const Vec3 &vec) const {
    Vec3 t = 2 * Cross(-v, vec);
    return vec + s * t + Cross(-v, t);
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
