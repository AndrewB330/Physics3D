#pragma once
#include <math/rand.hpp>

struct Color {
    // for some glut APIs
    float data[4];

    float r, g, b, a;

    Color(float r = 0.0, float g = 0.0, float b = 0.0, float a = 1.0);

    static Color FromHsl(double h, double s, double l);

    static Color GetRandomHsl(double s = 0.5, double l = 0.6);

private:
    static RandomGen rand;
};

Color operator*(const Color& lhs, float rhs);

struct Material {
    Color ambient;
    Color diffuse;
    Color specular;
};

struct PhysicsMaterial {
    double density;
};