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

    delta_impulse = Impulse();
}

void PhysObject::AddImpulse(Vec3 linear_impulse, Vec3 angular_impulse) {
    if (!fixed) {
        delta_impulse.linear_impulse += linear_impulse;
        delta_impulse.angular_impulse += angular_impulse;
    }
}

void PhysObject::SetPosition(Vec3 position) {
    collider->SetTranslation(position);
}

void PhysObject::SetRotation(Quat rotation) {
    collider->SetRotation(rotation);
    const Mat3 &rot = collider->GetRotationMat();
    inertia.moment_of_inertia_inv_global = rot * inertia.moment_of_inertia_inv * rot.T();
}

void PhysObject::SetFixed(bool val) {
    fixed = val;
    RecalculateInertia();
}

const Vec3 &PhysObject::GetPosition() const {
    return collider->GetTranslation();
}

const Quat &PhysObject::GetRotation() const {
    return collider->GetRotation();
}

void PhysObject::RecalculateInertia() {
    if (fixed) {
        inertia = Inertia();
    } else {
        inertia = ComputeInertia(std::const_pointer_cast<const Collider>(collider), phys_material);
        const Mat3 &rot = collider->GetRotationMat();
        inertia.moment_of_inertia_inv_global = rot * inertia.moment_of_inertia_inv * rot.T();
    }
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

Vec3 PhysObject::GetAccumulatedVelocity() const {
    return inertia.mass_inv * (impulse.linear_impulse + delta_impulse.linear_impulse);
}

Vec3 PhysObject::GetAccumulatedAngularVelocity() const {
    return inertia.moment_of_inertia_inv_global * (impulse.angular_impulse + delta_impulse.angular_impulse);
}

PhysObject::PhysObject(std::shared_ptr<Collider> collider, const PhysMaterial &material, bool fixed)
        : collider(std::move(collider)), phys_material(material), fixed(fixed) {
    RecalculateInertia();
}

std::shared_ptr<const Collider> PhysObject::GetCollider() const {
    return std::const_pointer_cast<const Collider>(collider);
}

std::unique_ptr<PhysObject>
CreateSphereObject(double radius, const PhysMaterial &mat, Vec3 position, Vec3 velocity, Quat rotation,
                   Vec3 angular_velocity, bool fixed) {
    auto object = std::make_unique<PhysObject>(
            CreateSphereCollider(radius, position, rotation),
            mat,
            fixed
    );
    object->SetVelocity(velocity);
    object->SetAngularVelocity(angular_velocity);
    return object;
}

std::unique_ptr<PhysObject>
CreateBoxObject(Vec3 size, const PhysMaterial &mat, Vec3 position, Vec3 velocity, Quat rotation, Vec3 angular_velocity,
                bool fixed) {
    auto object = std::make_unique<PhysObject>(
            CreateBoxCollider(size, position, rotation),
            mat,
            fixed
    );
    object->SetVelocity(velocity);
    object->SetAngularVelocity(angular_velocity);
    return object;
}
