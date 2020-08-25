#include "mat.h"
#include <memory>

Mat33::Mat33() {}

const T* Mat33::operator[](size_t i) const {
    return val[i];
}

T* Mat33::operator[](size_t i) {
    return val[i];
}

Mat33 Mat33::Inverse() const {
    // computes the inverse of a matrix m
    double det = val[0][0] * (val[1][1] * val[2][2] - val[2][1] * val[1][2]) -
        val[0][1] * (val[1][0] * val[2][2] - val[1][2] * val[2][0]) +
        val[0][2] * (val[1][0] * val[2][1] - val[1][1] * val[2][0]);

    double invdet = 1 / det;

    Mat33 minv; // inverse of matrix m
    minv[0][0] = (val[1][1] * val[2][2] - val[2][1] * val[1][2]) * invdet;
    minv[0][1] = (val[0][2] * val[2][1] - val[0][1] * val[2][2]) * invdet;
    minv[0][2] = (val[0][1] * val[1][2] - val[0][2] * val[1][1]) * invdet;
    minv[1][0] = (val[1][2] * val[2][0] - val[1][0] * val[2][2]) * invdet;
    minv[1][1] = (val[0][0] * val[2][2] - val[0][2] * val[2][0]) * invdet;
    minv[1][2] = (val[1][0] * val[0][2] - val[0][0] * val[1][2]) * invdet;
    minv[2][0] = (val[1][0] * val[2][1] - val[2][0] * val[1][1]) * invdet;
    minv[2][1] = (val[2][0] * val[0][1] - val[0][0] * val[2][1]) * invdet;
    minv[2][2] = (val[0][0] * val[1][1] - val[1][0] * val[0][1]) * invdet;

    return minv;
}

Mat33 operator*(const Mat33& a, const Mat33& b) {
    // Yes I know how to write loops
    Mat33 c;
    c[0][0] = a[0][0] * b[0][0] + a[0][1] * b[1][0] + a[0][2] * b[2][0];
    c[0][1] = a[0][0] * b[0][1] + a[0][1] * b[1][1] + a[0][2] * b[2][1];
    c[0][2] = a[0][0] * b[0][2] + a[0][1] * b[1][2] + a[0][2] * b[2][2];

    c[1][0] = a[1][0] * b[0][0] + a[1][1] * b[1][0] + a[1][2] * b[2][0];
    c[1][1] = a[1][0] * b[0][1] + a[1][1] * b[1][1] + a[1][2] * b[2][1];
    c[1][2] = a[1][0] * b[0][2] + a[1][1] * b[1][2] + a[1][2] * b[2][2];

    c[2][0] = a[2][0] * b[0][0] + a[2][1] * b[1][0] + a[2][2] * b[2][0];
    c[2][1] = a[2][0] * b[0][1] + a[2][1] * b[1][1] + a[2][2] * b[2][1];
    c[2][2] = a[2][0] * b[0][2] + a[2][1] * b[1][2] + a[2][2] * b[2][2];
    return c;
}

Vec3 operator*(const Mat33& lhs, const Vec3& rhs) {
    return Vec3(
        lhs[0][0] * rhs.x + lhs[0][1] * rhs.y + lhs[0][2] * rhs.z,
        lhs[1][0] * rhs.x + lhs[1][1] * rhs.y + lhs[1][2] * rhs.z,
        lhs[2][0] * rhs.x + lhs[2][1] * rhs.y + lhs[2][2] * rhs.z
    );
}

Mat33 operator*(const Mat33& lhs, T rhs) {
    Mat33 res = lhs;
    for (int i = 0; i < 9; i++) {
        *(&res.val[0][0] + i) *= rhs;
    }
    return res;
}

Mat33 operator*(T lhs, const Mat33& rhs) {
    return rhs * lhs;
}
