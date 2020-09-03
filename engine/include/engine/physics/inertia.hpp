#include <common/math/mat3.hpp>
#include <memory>
#include <engine/collision/collider.hpp>
#include "material.hpp"

/**
 * Contains all variables describing the body's inertia (mass, moment of inertia)
 */
struct Inertia {
    double mass = 1.0;
    double mass_inv = 0.0;

    /**
     * These matrices are given in local coordinates of the body (no transformation)
     */
    Mat3 moment_of_inertia = Mat3::Identity();
    Mat3 moment_of_inertia_inv = Mat3();

    /**  R * moment_of_inertia_inv * R^T  **/
    Mat3 moment_of_inertia_inv_global = Mat3();
};

/**
 * Compute inertia for some collider.
 * todo: it should be refactored. It only works for sphere and box
 */
Inertia ComputeInertia(const std::shared_ptr<const Collider>& shape, const PhysMaterial& material);

