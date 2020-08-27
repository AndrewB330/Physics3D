#include "../include/engine/model/objects/sphere.hpp"

Sphere::Sphere(double radius) :radius(radius) {
    TransformationUpdated();
}

const double& Sphere::GetRadius() const {
    return radius;
}

Vec3 Sphere::GetSupportingVector(Vec3 direction) const {
    return state.position + direction.Norm() * radius; // todo: delete Norm?
}

void Sphere::TransformationUpdated() {
    bbox.max.x = state.position.x + radius;
    bbox.max.y = state.position.y + radius;
    bbox.max.z = state.position.z + radius;
    bbox.min.x = state.position.x - radius;
    bbox.min.y = state.position.y - radius;
    bbox.min.z = state.position.z - radius;

    state.rotation_mat = state.rotation.Mat();
}

void Sphere::RecalculateInertia() {
    if (fixed) {
        inertia = ObjectInertia();
    } else {
        inertia.mass = 4.0 / 3 * PI * radius * radius * radius * physics_material.density;
        inertia.mass_inv = 1.0 / inertia.mass;

        inertia.moment_of_inertia = Mat3();
        inertia.moment_of_inertia[0][0] = 2 / 5.0 * inertia.mass * radius * radius;
        inertia.moment_of_inertia[1][1] = 2 / 5.0 * inertia.mass * radius * radius;
        inertia.moment_of_inertia[2][2] = 2 / 5.0 * inertia.mass * radius * radius;

        inertia.moment_of_inertia_inv = inertia.moment_of_inertia.Inverse();
    }
}
