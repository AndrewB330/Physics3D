#pragma once
#include "color.h"

struct Material {
    Color ambient;
    Color diffuse;
    Color specular = Color(0.1f, 0.1f, 0.1f, 1.0f);
};

struct PhysicsMaterial {
    double density;
};