#pragma once
#include "../math/vec3.h"
#include "../math/mat.h"
#include "../math/quat.h"
#include "material.h"
#include "bounding_box.h"

struct ObjectInertia {
    double mass = 0.0;
    double mass_inv = 0.0;

    Mat33 moment_of_inertia = Mat33();
    Mat33 moment_of_inertia_inv = Mat33();
};

struct ObjectState {
    Vec3 position = Vec3();
    Vec3 impulse = Vec3();

    Quat rotation = Quat();
    Vec3 angular_momentum = Vec3();
};

struct Variables {
    Vec3 delta_impulse;
    Vec3 delta_angular_momentum;
};

class Object {
public:
    Object() = default;
    virtual ~Object() = default;

    const ObjectState& GetState() const;

    const ObjectInertia& GetInertia() const;

    const Material& GetMaterial() const;

    const PhysicsMaterial& GetPhysicsMaterial() const;

    const BoundingBox& GetBBox() const;

    void ApplyChanges(double dt);

    void AddImpulse(Vec3 impulse, Vec3 point);

    virtual Vec3 GetSupportingPoint(Vec3 direction) const = 0;

    void UpdateMaterial(const Material& material);

    void UpdatePhysicsMaterial(const PhysicsMaterial& material);

    void UpdateState(Vec3 position, Vec3 velocity, Quat rotation, Vec3 angular_momentum);

    void ResetVariables();

    void SetFixed(bool val = true);

protected:
    virtual void TransformationUpdated() = 0;

    virtual void RecalculateInertia() = 0;

    bool fixed = true;

    Material material;
    PhysicsMaterial physics_material;

    ObjectInertia inertia;
    ObjectState state;

    Variables variables;

    BoundingBox bbox;
};