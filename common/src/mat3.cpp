#include <common/math/mat3.hpp>
#include <memory>

Mat3::Mat3() {}

const T* Mat3::operator[](size_t i) const {
    return val[i];
}

T* Mat3::operator[](size_t i) {
    return val[i];
}

Mat3 Mat3::Inverse() const {
    // computes the inverse of a matrix m
    double det = val[0][0] * (val[1][1] * val[2][2] - val[2][1] * val[1][2]) -
        val[0][1] * (val[1][0] * val[2][2] - val[1][2] * val[2][0]) +
        val[0][2] * (val[1][0] * val[2][1] - val[1][1] * val[2][0]);

    double invdet = 1 / det;

    Mat3 minv; // inverse of matrix m
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

Mat3 Mat3::Identity() {
    Mat3 res;
    res[0][0] = res[1][1] = res[2][2] = 1;
    return res;
}

Mat3 Mat3::T() const {
    Mat3 m;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            m[i][j] = val[j][i];
        }
    }
    return m;
}

Mat3 operator*(const Mat3& a, const Mat3& b) {
    // Yes I know how to write loops
    Mat3 c;
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

Vec3 operator*(const Mat3& lhs, const Vec3& rhs) {
    return Vec3(
        lhs[0][0] * rhs.x + lhs[0][1] * rhs.y + lhs[0][2] * rhs.z,
        lhs[1][0] * rhs.x + lhs[1][1] * rhs.y + lhs[1][2] * rhs.z,
        lhs[2][0] * rhs.x + lhs[2][1] * rhs.y + lhs[2][2] * rhs.z
    );
}

Mat3 operator*(const Mat3& lhs, T rhs) {
    Mat3 res = lhs;
    for (int i = 0; i < 9; i++) {
        *(&res.val[0][0] + i) *= rhs;
    }
    return res;
}

Mat3 operator*(T lhs, const Mat3& rhs) {
    return rhs * lhs;
}
