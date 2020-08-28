#include <engine/physics/object.hpp>

const Inertia &PhysObject::GetInertia() const {
    return inertia;
}

const PhysMaterial &PhysObject::GetPhysMaterial() const {
    return phys_material;
}

void PhysObject::ApplyChanges(double dt) {
    if (!fixed) {
        Vec3 next_position =
                shape->GetTranslation() +
                impulse.linear_impulse * inertia.mass_inv * dt +
                delta_impulse.linear_impulse * inertia.mass_inv * dt * 0.5;

        Quat next_rotation = (shape->GetRotation() +
                              (0.5 * inertia.moment_of_inertia_inv * impulse.angular_momentum *
                               shape->GetRotation()) * dt).Norm();
        next_rotation = (next_rotation +
                         (0.25 * inertia.moment_of_inertia_inv * delta_impulse.angular_momentum *
                          next_rotation) * dt).Norm();

        impulse.linear_impulse += delta_impulse.linear_impulse;
        impulse.angular_momentum += delta_impulse.angular_momentum;

        shape->SetRotation(next_rotation);
        shape->SetTranslation(next_position);
    } else {
        Vec3 next_position = shape->GetTranslation() + impulse.linear_impulse * dt;
        Quat next_rotation = (shape->GetRotation() +
                              (0.5 * impulse.angular_momentum * shape->GetRotation() * dt)).Norm();
        shape->SetRotation(next_rotation);
        shape->SetTranslation(next_position);
    }
    impulse = Impulse();
}

void PhysObject::AddImpulse(Vec3 impulse_value, Vec3 point) {
    if (!fixed) {
        delta_impulse.linear_impulse += impulse_value;
        delta_impulse.angular_momentum += Cross(point - shape->GetTranslation(), impulse_value);
    }
}

void PhysObject::SetPosition(Vec3 position) {
    shape->SetTranslation(position);
}

void PhysObject::SetRotation(Quat rotation) {
    shape->SetRotation(rotation);
}

void PhysObject::ResetDeltaImpulse() {
    delta_impulse = Impulse();
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
        impulse.angular_momentum = angular_velocity;
    } else {
        impulse.angular_momentum = inertia.moment_of_inertia * angular_velocity;
    }
}

Vec3 PhysObject::GetVelocityAtPoint(const Vec3 &point) const {
    if (fixed) {
        return Vec3();
    }
    Mat3 rot = GetRotation().Mat();
    Mat3 I = rot * inertia.moment_of_inertia_inv * rot.T();

    return impulse.linear_impulse * inertia.mass_inv +
           Cross(I * impulse.angular_momentum, point - GetPosition()) +
           delta_impulse.linear_impulse * inertia.mass_inv +
           Cross(I * delta_impulse.angular_momentum, point - GetPosition());
}
