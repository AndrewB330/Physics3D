#pragma once

struct Color {
    float r, g, b, a;
    float data[4];
    Color(float r = 0.0, float g = 0.0, float b = 0.0, float a = 1.0);
};