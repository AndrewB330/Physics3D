#pragma once
using T = double;
struct Mat33 {
    T val[3][3] = { {0,0,0},{0,0,0},{0,0,0} };
    Mat33();

    const T* operator[](size_t i) const;
    T* operator[](size_t i);
};

Mat33 operator*(const Mat33& lhs, const Mat33& rhs);