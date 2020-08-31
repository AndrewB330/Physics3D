#pragma once

#include <math/vec3.hpp>
#include <math/mat3.hpp>
#include <math/quat.hpp>
#include <engine/physics/material.hpp>
#include <engine/physics/inertia.hpp>
#include <engine/collision/collider.hpp>

struct Impulse {
    Vec3 linear_impulse = Vec3();
    Vec3 angular_impulse = Vec3();
};

class PhysObject {
public:
    PhysObject(std::shared_ptr<Collider> collider, const PhysMaterial &material, bool fixed = false);

    virtual ~PhysObject() = default;

    const Inertia &GetInertia() const;

    const PhysMaterial &GetPhysMaterial() const;

    void ApplyChanges(double dt);

    void AddImpulse(Vec3 linear_impulse, Vec3 angular_impulse);

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

    std::shared_ptr<const Collider> GetCollider() const;

protected:

    void RecalculateInertia();

    bool fixed = true;

    PhysMaterial phys_material;

    Inertia inertia;

    Impulse impulse;
    Impulse delta_impulse;

    std::shared_ptr<Collider> collider;
};

std::unique_ptr<PhysObject>
CreateSphereObject(double radius, const PhysMaterial &mat, Vec3 position = Vec3(), Vec3 velocity = Vec3(),
                   Quat rotation = Quat::Identity(), Vec3 angular_velocity = Vec3(),
                   bool fixed = false);

std::unique_ptr<PhysObject>
CreateBoxObject(Vec3 size, const PhysMaterial &mat, Vec3 position = Vec3(), Vec3 velocity = Vec3(),
                Quat rotation = Quat::Identity(), Vec3 angular_velocity = Vec3(),
                bool fixed = false);
