#pragma once

#include <math/vec3.hpp>
#include <math/mat3.hpp>
#include <math/quat.hpp>
#include <engine/physics/material.hpp>
#include <engine/geometry/shape.hpp>
#include <engine/physics/inertia.hpp>

struct Impulse {
    Vec3 linear_impulse = Vec3();
    Vec3 angular_impulse = Vec3();
};

class PhysObject {
public:
    PhysObject(std::unique_ptr<ConvexShape> shape, PhysMaterial material);

    virtual ~PhysObject() = default;

    const Inertia &GetInertia() const;

    const PhysMaterial &GetPhysMaterial() const;

    void ApplyChanges(double dt);

    void AddImpulse(Vec3 linear_impulse, Vec3 angular_impulse);

    void AddPseudoVelocity(const Vec3& pseudo_velocity);

    void SetPosition(Vec3 position);

    void SetRotation(Quat rotation);

    void SetVelocity(const Vec3 &velocity);

    void SetAngularVelocity(const Vec3 &angular_velocity);

    const Vec3 &GetPosition() const;

    const Quat &GetRotation() const;

    Vec3 GetVelocity() const;

    Vec3 GetAngularVelocity() const;

    Vec3 GetAccumulatedVelocity() const;

    Vec3 GetAccumulatedAngularVelocity() const;

    void SetFixed(bool val = true);

    bool IsFixed() const;

    const ConvexShape *GetShape() const;

protected:

    void RecalculateInertia();

    bool fixed = true;

    PhysMaterial phys_material;

    Inertia inertia;

    Impulse impulse;
    Impulse delta_impulse;

    Vec3 pseudo_velocity;

    std::unique_ptr<ConvexShape> shape;
};