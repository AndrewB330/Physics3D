#include <math/mat3.hpp>
#include <engine/geometry/shape.hpp>
#include "material.hpp"

struct Inertia {
    double mass = 1.0;
    double mass_inv = 0.0;

    Mat3 moment_of_inertia = Mat3::Identity();
    Mat3 moment_of_inertia_inv = Mat3();
};

Inertia ComputeInertia(const Shape* shape, const PhysMaterial& material);

