
using T = double;

struct Vec2 {
    T x = 0;
    T y = 0;

    Vec2() = default;

    Vec2(T x, T y);

    T LenSqr() const;

    T Len() const;

    Vec2 Norm() const;

    Vec2 &operator+=(const Vec2 &other);

    Vec2 &operator-=(const Vec2 &other);

    Vec2 &operator*=(T k);

    Vec2 &operator/=(T k);
};

Vec2 operator+(const Vec2 &lhs, const Vec2 &rhs);

Vec2 operator-(const Vec2 &lhs, const Vec2 &rhs);

Vec2 operator*(const Vec2 &lhs, T rhs);

Vec2 operator*(T lhs, const Vec2 &rhs);

Vec2 operator/(const Vec2 &lhs, T rhs);

Vec2 operator-(const Vec2 &v);


T Dot(const Vec2 &lhs, const Vec2 &rhs);

T Cross(const Vec2 &lhs, const Vec2 &rhs);