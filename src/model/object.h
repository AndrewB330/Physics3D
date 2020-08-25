#pragma once
#include "../math/vec3.h"
#include "../math/mat.h"
#include "../math/quat.h"
#include "material.h"

struct ObjectProperties {
    bool fixed = false;

    double mass = 0.0;
    double mass_inv = 0.0;

    Mat33 inertia = Mat33();
    Mat33 inertia_inv = Mat33();
};

struct ObjectState {
    Vec3 position = Vec3();
    Vec3 velocity = Vec3();

    Quat rotation = Quat();
    Vec3 angular_velocity = Vec3();
};

class Object {
public:
    Object() = default;
    virtual ~Object() = default;

    ObjectProperties properties;
    ObjectState state;

    Material material;
protected:
};