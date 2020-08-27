#include "../include/engine/model/object.hpp"

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
        state.position += delta_state.delta_impulse * inertia.mass_inv * dt * 0.5;
        state.impulse += delta_state.delta_impulse;

        state.rotation = (state.rotation + (0.5 * inertia.moment_of_inertia_inv * state.angular_momentum * state.rotation)).Norm();
        state.rotation = (state.rotation + (0.25 * inertia.moment_of_inertia_inv * delta_state.delta_angular_momentum * state.rotation)).Norm();
        state.angular_momentum += delta_state.delta_angular_momentum;

        TransformationUpdated();
    } else {
        state.position += state.impulse * dt; 
        state.rotation = (state.rotation + (0.5 * state.angular_momentum * state.rotation)).Norm();

        TransformationUpdated();
    }
}

void Object::AddImpulse(Vec3 impulse, Vec3 point) {
    if (!fixed) {
        delta_state.delta_impulse += impulse;
        delta_state.delta_angular_momentum += Cross(point - state.position, impulse);
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

void Object::SetPosition(Vec3 position) {
    state.position = position;

    TransformationUpdated();
}

void Object::SetRotation(Quat rotation) {
    state.rotation = rotation;

    TransformationUpdated();
}

void Object::ResetDeltaState() {
    delta_state = ObjectDeltaState();
}

void Object::SetFixed(bool val) {
    fixed = val;
    RecalculateInertia();
}
