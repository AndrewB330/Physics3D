#pragma once

#include <math/vec3.hpp>
#include <math/mat3.hpp>
#include <math/quat.hpp>
#include <engine/physics/material.hpp>
#include <engine/geometry/shape.hpp>
#include <engine/physics/inertia.hpp>

struct Impulse {
    Vec3 linear_impulse = Vec3();
    Vec3 angular_momentum = Vec3();
};

class PhysObject {
public:
    PhysObject(std::unique_ptr<ConvexShape> shape, PhysMaterial material);

    virtual ~PhysObject() = default;

    [[nodiscard]] const Inertia &GetInertia() const;

    [[nodiscard]] const PhysMaterial &GetPhysMaterial() const;

    void ApplyChanges(double dt);

    void AddImpulse(Vec3 impulse_value, Vec3 point);

    void SetPosition(Vec3 position);

    void SetRotation(Quat rotation);

    void SetVelocity(const Vec3& velocity);

    void SetAngularVelocity(const Vec3& angular_velocity);

    const Vec3 &GetPosition() const;

    const Quat &GetRotation() const;

    void ResetDeltaImpulse();

    void SetFixed(bool val = true);

    const ConvexShape* GetShape() const;

    Vec3 GetVelocityAtPoint(const Vec3& point) const;

protected:

    void RecalculateInertia();

    bool fixed = true;

    PhysMaterial phys_material;

    Inertia inertia;
    Impulse impulse;

    Impulse delta_impulse;

    std::unique_ptr<ConvexShape> shape;
};