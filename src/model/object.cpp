#include "object.h"

const ObjectState& Object::GetState() const {
    return state;
}

const ObjectInertia& Object::GetInertia() const {
    return inertia;
}

const Material& Object::GetMaterial() const {
    return material;
}

const PhysicsMaterial& Object::GetPhysicsMaterial() const {
    return physics_material;
}

const BoundingBox& Object::GetBBox() const {
    return bbox;
}

void Object::ApplyChanges(double dt) {
    if (!fixed) {
        state.position += state.impulse * inertia.mass_inv * dt;
        state.position += variables.delta_impulse * inertia.mass_inv * dt * 0.5;
        state.impulse += variables.delta_impulse;

        state.rotation = (state.rotation + (0.5 * inertia.moment_of_inertia_inv * state.angular_momentum * state.rotation)).Norm();
        state.rotation = (state.rotation + (0.25 * inertia.moment_of_inertia_inv * variables.delta_angular_momentum * state.rotation)).Norm();
        state.angular_momentum += variables.delta_angular_momentum;

        TransformationUpdated();
    } else {
        state.position += state.impulse * inertia.mass_inv * dt; 
        state.rotation = (state.rotation + (0.5 * inertia.moment_of_inertia_inv * state.angular_momentum * state.rotation)).Norm();

        TransformationUpdated();
    }
}

void Object::AddImpulse(Vec3 impulse, Vec3 point) {
    if (!fixed) {
        variables.delta_impulse += impulse;
        variables.delta_angular_momentum += Cross(point - state.position, impulse);
    }
}

void Object::UpdateMaterial(const Material& material) {
    this->material = material;
}

void Object::UpdatePhysicsMaterial(const PhysicsMaterial& material) {
    this->physics_material = material;
}

void Object::UpdateState(Vec3 position, Vec3 velocity, Quat rotation, Vec3 angular_momentum) {
    state.position = position;
    state.impulse = velocity;
    state.rotation = rotation;
    state.angular_momentum = angular_momentum;

    TransformationUpdated();
}

void Object::ResetVariables() {
    variables = Variables();
}

void Object::SetFixed(bool val) {
    fixed = val;
    RecalculateInertia();
}
