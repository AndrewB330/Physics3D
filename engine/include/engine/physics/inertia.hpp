#include <math/mat3.hpp>
#include <memory>
#include <engine/collision/collider.hpp>
#include "material.hpp"

struct Inertia {
    double mass = 1.0;
    double mass_inv = 0.0;

    Mat3 moment_of_inertia = Mat3::Identity();
    Mat3 moment_of_inertia_inv = Mat3();

    /**  R * moment_of_inertia_inv * R^T  **/
    Mat3 moment_of_inertia_inv_global = Mat3();
};

Inertia ComputeInertia(const std::shared_ptr<const Collider>& shape, const PhysMaterial& material);

