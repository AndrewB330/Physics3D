#pragma once
#include <math/vec3.hpp>
#include <math/mat3.hpp>
#include <math/quat.hpp>
#include "material.hpp"
#include "bounding_box.hpp"

struct ObjectInertia {
    double mass = 0.0;
    double mass_inv = 0.0;

    Mat3 moment_of_inertia = Mat3();
    Mat3 moment_of_inertia_inv = Mat3();
};

struct ObjectState {
    Vec3 position = Vec3();
    Vec3 impulse = Vec3();

    Quat rotation = Quat::Identity();
    Vec3 angular_momentum = Vec3();
    
    // Auxilary
    Mat3 rotation_mat = Mat3::Identity();
};

struct ObjectDeltaState {
    Vec3 delta_impulse;
    Vec3 delta_angular_momentum;
};

class Object {
public:

    bool collided = false;

    Object() = default;
    virtual ~Object() = default;

    const ObjectState& GetState() const;

    const ObjectInertia& GetInertia() const;

    const Material& GetMaterial() const;

    const PhysicsMaterial& GetPhysicsMaterial() const;

    const BoundingBox& GetBBox() const;

    void ApplyChanges(double dt);

    void AddImpulse(Vec3 impulse, Vec3 point);

    virtual Vec3 GetSupportingVector(Vec3 direction) const = 0;

    void UpdateMaterial(const Material& material);

    void UpdatePhysicsMaterial(const PhysicsMaterial& material);

    void UpdateState(Vec3 position, Vec3 velocity, Quat rotation, Vec3 angular_momentum);

    void SetPosition(Vec3 position);

    void SetRotation(Quat rotation);

    void ResetDeltaState();

    void SetFixed(bool val = true);

protected:
    virtual void TransformationUpdated() = 0;

    virtual void RecalculateInertia() = 0;

    bool fixed = true;

    Material material;
    PhysicsMaterial physics_material;

    ObjectInertia inertia;
    ObjectState state;
    ObjectDeltaState delta_state;

    BoundingBox bbox;
};