#include <engine/physics/inertia.hpp>

Inertia ComputeInertia(const Shape *shape, const PhysMaterial &material) {
    if (auto s = dynamic_cast<const SphereShape*>(shape)) {
        Inertia inertia;
        inertia.mass = 4.0 / 3 * PI * s->GetRadius() * s->GetRadius() * s->GetRadius() * material.density;
        inertia.mass_inv = 1.0 / inertia.mass;

        inertia.moment_of_inertia = Mat3::Identity() * (inertia.mass * s->GetRadius() * s->GetRadius() / 5.0);
        inertia.moment_of_inertia_inv = inertia.moment_of_inertia.Inverse();
        return inertia;
    }
    if (auto b = dynamic_cast<const BoxShape*>(shape)) {
        Inertia inertia;
        Vec3 size = b->GetSize();
        inertia.mass = size.x * size.y * size.z * material.density;
        inertia.mass_inv = 1.0 / inertia.mass;

        inertia.moment_of_inertia = Mat3();
        inertia.moment_of_inertia[0][0] = inertia.mass * (size.y * size.y + size.z * size.z) / 12;
        inertia.moment_of_inertia[1][1] = inertia.mass * (size.x * size.x + size.z * size.z) / 12;
        inertia.moment_of_inertia[2][2] = inertia.mass * (size.y * size.y + size.x * size.x) / 12;
        inertia.moment_of_inertia_inv = inertia.moment_of_inertia.Inverse();
        return inertia;
    }
    return Inertia();
}
