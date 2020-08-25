#include "color.h"

Color::Color(float r, float g, float b, float a) :r(r), g(g), b(b), a(a) {
    data[0] = r;
    data[1] = g;
    data[2] = b;
    data[3] = a;
}
