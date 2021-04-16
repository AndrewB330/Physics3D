#pragma once

#include <engine/physics/material.hpp>
#include <engine/physics/inertia.hpp>
#include <engine/collision/collider.hpp>

struct Impulse {
    Vec3 linear_impulse = Vec3();
    Vec3 angular_impulse = Vec3();
};

Impulse operator+(const Impulse& lhs, const Impulse& rhs);

/**
 * The main building part of the simulation.
 * Represents some body with a given collider and some physical properties (inertia, impulse, velocity...)
 */
class PhysObject: public Transformable {
public:
    PhysObject(std::shared_ptr<Collider> collider, const PhysMaterial &material, bool fixed = false);

    int GetId() const;

    void SetId(int id);

    virtual ~PhysObject() = default;

    const Inertia &GetInertia() const;

    const PhysMaterial &GetPhysMaterial() const;

    /**
     * Call it once per tick, to apply all changes (solver results)
     * @param dt - elapsed time since the last tick
     */
    void ApplyChanges(double dt);

    /**
     * Apply some impulse. It is not applied immediately, it is accumulated in delta_impulse
     */
    void AddImpulse(Vec3 linear_impulse, Vec3 angular_impulse);

    /**
     * Apply some pseudo impulse. It is not applied immediately, it is accumulated in pseudo_impulse.
     * Used for creating a repulsion between bodies.
     */
    void AddPseudoImpulse(Vec3 linear_impulse, Vec3 angular_impulse);

    void SetVelocity(const Vec3 &velocity);

    void SetAngularVelocity(const Vec3 &angular_velocity);

    Vec3 GetVelocity() const;

    Vec3 GetAngularVelocity() const;

    Vec3 GetAccumulatedVelocity() const;

    Vec3 GetAccumulatedAngularVelocity() const;

    /**
     * Fixed body is not affected by gravity and has infinite inertia
     */
    bool IsFixed() const;

    /**
     * Body becomes sleeping if it was not moved (or moved too little) for some period of time
     */
    bool IsSleeping() const;

    void SetSleeping(bool val = true);

    void SetFixed(bool val = true);

    std::shared_ptr<const Collider> GetCollider() const;

    /**
     * Transformable interface implementation
     */

    const Vec3 &GetTranslation() const override;

    const Quat &GetRotation() const override;

    void SetTranslation(const Vec3& translate) override;

    void SetRotation(const Quat& rotation) override;

    void Translate(const Vec3& translate) override;

    void Rotate(const Quat& rotation) override;

protected:

    int id;

    /**
     * Should be called once, in constructor (or when IsFixed status changed)
     */
    void RecalculateInertia();

    bool fixed = false;
    bool sleeping = false;

    // number of ticks that body was not moving (within some threshold)
    int no_motion_ticks = 0;

    PhysMaterial phys_material;

    Inertia inertia;

    Impulse impulse;
    Impulse delta_impulse;
    Impulse pseudo_impulse;

    std::shared_ptr<Collider> collider;
};

typedef std::shared_ptr<PhysObject> PhysObjectPtr;

// Some helper function, todo: improve interface, decrease code duplication
std::unique_ptr<PhysObject>
CreateSphereObject(double radius, const PhysMaterial &mat, Vec3 position = Vec3(), Vec3 velocity = Vec3(),
                   Quat rotation = Quat::Identity(), Vec3 angular_velocity = Vec3(),
                   bool fixed = false);

std::unique_ptr<PhysObject>
CreateBoxObject(Vec3 size, const PhysMaterial &mat, Vec3 position = Vec3(), Vec3 velocity = Vec3(),
                Quat rotation = Quat::Identity(), Vec3 angular_velocity = Vec3(),
                bool fixed = false);
