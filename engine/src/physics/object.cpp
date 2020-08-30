#include <engine/physics/object.hpp>

const Inertia &PhysObject::GetInertia() const {
    return inertia;
}

const PhysMaterial &PhysObject::GetPhysMaterial() const {
    return phys_material;
}

void PhysObject::ApplyChanges(double dt) {
    if (!fixed) {
        impulse.linear_impulse += delta_impulse.linear_impulse;
        impulse.angular_impulse += delta_impulse.angular_impulse;
    }

    SetPosition(GetPosition() + GetVelocity() * dt);
    SetRotation(GetRotation() + (0.5 * GetAngularVelocity() * GetRotation()) * dt);

    SetPosition(GetPosition() + pseudo_velocity * dt);

    delta_impulse = Impulse();
    pseudo_velocity = Vec3();
}

void PhysObject::AddImpulse(Vec3 linear_impulse, Vec3 angular_impulse) {
    if (!fixed) {
        delta_impulse.linear_impulse += linear_impulse;
        delta_impulse.angular_impulse += angular_impulse;
    }
}

void PhysObject::SetPosition(Vec3 position) {
    shape->SetTranslation(position);
}

void PhysObject::SetRotation(Quat rotation) {
    shape->SetRotation(rotation);
    const Mat3& rot = shape->GetRotationMat();
    inertia.moment_of_inertia_inv_global = rot * inertia.moment_of_inertia_inv * rot.T();
}

void PhysObject::SetFixed(bool val) {
    fixed = val;
    RecalculateInertia();
}

const Vec3 &PhysObject::GetPosition() const {
    return shape->GetTranslation();
}

const Quat &PhysObject::GetRotation() const {
    return shape->GetRotation();
}

PhysObject::PhysObject(std::unique_ptr<ConvexShape> shape, PhysMaterial material)
        : shape(std::move(shape)), phys_material(material) {
    RecalculateInertia();
}

void PhysObject::RecalculateInertia() {
    if (fixed) {
        inertia = Inertia();
    } else {
        inertia = ComputeInertia(shape.get(), phys_material);
    }
}

const ConvexShape *PhysObject::GetShape() const {
    return shape.get();
}

void PhysObject::SetVelocity(const Vec3 &velocity) {
    if (fixed) {
        impulse.linear_impulse = velocity;
    } else {
        impulse.linear_impulse = velocity * inertia.mass;
    }
}

void PhysObject::SetAngularVelocity(const Vec3 &angular_velocity) {
    if (fixed) {
        impulse.angular_impulse = angular_velocity;
    } else {
        impulse.angular_impulse = inertia.moment_of_inertia * angular_velocity;
    }
}

bool PhysObject::IsFixed() const {
    return fixed;
}

Vec3 PhysObject::GetVelocity() const {
    return inertia.mass_inv * impulse.linear_impulse;
}

Vec3 PhysObject::GetAngularVelocity() const {
    return inertia.moment_of_inertia_inv_global * impulse.angular_impulse;
}

void PhysObject::AddPseudoVelocity(const Vec3 &pseudo_velocity_) {
    pseudo_velocity += pseudo_velocity_;
}

Vec3 PhysObject::GetAccumulatedVelocity() const {
    return inertia.mass_inv * (impulse.linear_impulse + delta_impulse.linear_impulse) + pseudo_velocity;
}

Vec3 PhysObject::GetAccumulatedAngularVelocity() const {
    return inertia.moment_of_inertia_inv_global * (impulse.angular_impulse + delta_impulse.angular_impulse);
}
