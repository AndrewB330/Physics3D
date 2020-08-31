#include <engine/physics/inertia.hpp>

Inertia ComputeInertia(const std::shared_ptr<const Collider>& collider, const PhysMaterial &material) {
    auto orig = collider->GetOrigShape();
    if (auto s = dynamic_cast<const SphereShape*>(orig)) {
        Inertia inertia;
        double radius = s->GetSupportingVector(Vec3(1,0,0)).Len();
        inertia.mass = 4.0 / 3 * PI * radius * radius * radius * material.GetDensity();
        inertia.mass_inv = 1.0 / inertia.mass;

        inertia.moment_of_inertia = Mat3::Identity() * (inertia.mass * radius * radius / 5.0);
        inertia.moment_of_inertia_inv = inertia.moment_of_inertia.Inverse();
        inertia.moment_of_inertia_inv_global = inertia.moment_of_inertia_inv;
        return inertia;
    } else {
        Inertia inertia;
        Vec3 size = orig->GetSupportingVector(Vec3(1,1,1)) * 2;
        inertia.mass = size.x * size.y * size.z * material.GetDensity();
        inertia.mass_inv = 1.0 / inertia.mass;

        inertia.moment_of_inertia = Mat3();
        inertia.moment_of_inertia[0][0] = inertia.mass * (size.y * size.y + size.z * size.z) / 12;
        inertia.moment_of_inertia[1][1] = inertia.mass * (size.x * size.x + size.z * size.z) / 12;
        inertia.moment_of_inertia[2][2] = inertia.mass * (size.y * size.y + size.x * size.x) / 12;
        inertia.moment_of_inertia_inv = inertia.moment_of_inertia.Inverse();
        inertia.moment_of_inertia_inv_global = inertia.moment_of_inertia_inv;
        return inertia;
    }
}
