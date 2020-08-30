#pragma once

#include <engine/geometry/collision.hpp>
#include "object.hpp"

class Constraint {
public:
    virtual ~Constraint() = default;

    virtual void Solve() = 0;
};

class CollisionConstraint : public Constraint {
public:
    ~CollisionConstraint() override = default;

    CollisionConstraint(PhysObject *a, PhysObject *b, Collision collision);

    void Solve() override;

private:
    PhysObject *a;
    PhysObject *b;

    Vec3 GetRelativeVelocity() const;

    Vec3 resulting_velocity;

    Vec3 lever_a;
    Vec3 lever_b;

    Vec3 normal;
    Vec3 tangent_normal;

    double effective_mass;
    double tangent_effective_mass;

    double penetration;

    double friction;

    double accumulated_lambda = 0.0;

    bool tangent_normal_found;
};

class SpringConstraint: public Constraint {
public:
    ~SpringConstraint() override = default;

    SpringConstraint(PhysObject *a, PhysObject *b, Vec3 bind_a, Vec3 bind_b, double target_length, double stiffness = 1.0, double amortizing = 0.0);

    void Solve() override;

private:

    PhysObject *a;
    PhysObject *b;

    Vec3 bind_a;
    Vec3 bind_b;

    double target_length;
    double stiffness;
    double amortizing;
};
