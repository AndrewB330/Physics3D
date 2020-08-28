#include <engine/physics/constraints.hpp>

Constraint::Constraint(PhysObject *a, PhysObject *b, Collision collision)
        : a(a), b(b), collision(collision) {}

Vec3 Constraint::GetImpulse() const {
    Vec3 av = a->GetVelocityAtPoint(collision.point);
    Vec3 bv = b->GetVelocityAtPoint(collision.point);

    Mat3 rota = a->GetRotation().Mat();
    Mat3 Ia = rota * a->GetInertia().moment_of_inertia_inv * rota.T();
    Mat3 rotb = b->GetRotation().Mat();
    Mat3 Ib = rotb * a->GetInertia().moment_of_inertia_inv * rotb.T();
    Vec3 n = collision.normal;
    Vec3 ap = collision.point - a->GetPosition();
    Vec3 bp = collision.point - b->GetPosition();
    double M = a->GetInertia().mass_inv + Dot(Cross(Ia * Cross(ap, n), ap), n) +
               b->GetInertia().mass_inv + Dot(Cross(Ib * Cross(bp, n), bp), n);
    double impulse = (Dot(av, n) - Dot(bv, n)) / M;

    if (impulse > 0) {
        return -impulse * n;
    }
    return Vec3();
}
