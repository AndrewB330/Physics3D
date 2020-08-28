#include <viewer/material.hpp>

RandomGen Color::rand = RandomGen(0);

Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {
    data[0] = r;
    data[1] = g;
    data[2] = b;
    data[3] = a;
}

Color Color::FromHsl(double h, double s, double l) {
    if (h > 0 || h <= 360) {
        h = fmod(h, 360);
    }
    if (h < 0) {
        h += 360.;
    }
    double c = fabs(1 - fabs(2 * l - 1)) * s;
    double x = c * (1 - fabs(fmod(h / 60, 2) - 1));
    double m = l - c / 2;
    if (h < 60) {
        return Color(c + m, x + m, m);
    }
    if (h < 120) {
        return Color(x + m, c + m, m);
    }
    if (h < 180) {
        return Color(m, c + m, x + m);
    }
    if (h < 240) {
        return Color(m, x + m, c + m);
    }
    if (h < 300) {
        return Color(x + m, m, c + m);
    }
    return Color(c + m, m, x + m);
}

Color Color::GetRandomHsl(double s, double l) {
    return Color::FromHsl(Color::rand.get_int(0, 359), s, l);
}

Color operator*(const Color &lhs, float rhs) {
    return Color(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs);
}

RenderMaterial RenderMaterial::Default(const Color &color) {
    return {
            color * 0.5,
            color * 1.0,
            Color(0.1, 0.1, 0.1)
    };
}
