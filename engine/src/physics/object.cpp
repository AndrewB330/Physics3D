#include <engine/physics/object.hpp>

Impulse operator+(const Impulse &lhs, const Impulse &rhs) {
    return Impulse{lhs.linear_impulse + rhs.linear_impulse, lhs.angular_impulse + rhs.angular_impulse};
}

const Inertia &PhysObject::GetInertia() const {
    return inertia;
}

const PhysMaterial &PhysObject::GetPhysMaterial() const {
    return phys_material;
}

void PhysObject::ApplyChanges(double dt) {
    if (GetAccumulatedVelocity().Len() < 0.01 && GetAccumulatedAngularVelocity().Len() < 0.01) {
        no_motion_ticks++;
    } else {
        no_motion_ticks = 0;
    }
    if (no_motion_ticks > 5) {
        SetSleeping(true);
    }

    if (sleeping) {
        delta_impulse = Impulse();
        pseudo_impulse = Impulse();
        return;
    }

    if (!fixed) {
        impulse.linear_impulse += delta_impulse.linear_impulse;
        impulse.angular_impulse += delta_impulse.angular_impulse;
        delta_impulse = Impulse();
    }
    Translate(GetAccumulatedVelocity() * dt);
    SetRotation(GetRotation() + (0.5 * GetAccumulatedAngularVelocity() * GetRotation()) * dt);

    pseudo_impulse = Impulse();
}

void PhysObject::AddImpulse(Vec3 linear_impulse, Vec3 angular_impulse) {
    if (!fixed && !sleeping) {
        delta_impulse.linear_impulse += linear_impulse;
        delta_impulse.angular_impulse += angular_impulse;
    }
}

void PhysObject::AddPseudoImpulse(Vec3 linear_impulse, Vec3 angular_impulse) {
    if (!fixed && !sleeping) {
        pseudo_impulse.linear_impulse += linear_impulse;
        pseudo_impulse.angular_impulse += angular_impulse;
    }
}

void PhysObject::SetFixed(bool val) {
    fixed = val;
    RecalculateInertia();
}

void PhysObject::SetSleeping(bool val) {
    sleeping = val;
}

void PhysObject::RecalculateInertia() {
    if (fixed) {
        inertia = Inertia();
    } else {
        inertia = ComputeInertia(std::const_pointer_cast<const Collider>(collider), phys_material);
        const Mat3 &rot = collider->GetRotation().Mat();
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
    auto acc_imp = impulse + delta_impulse + pseudo_impulse;
    return inertia.mass_inv * acc_imp.linear_impulse;
}

Vec3 PhysObject::GetAccumulatedAngularVelocity() const {
    return inertia.moment_of_inertia_inv_global * (impulse + delta_impulse + pseudo_impulse).angular_impulse;
}

PhysObject::PhysObject(std::shared_ptr<Collider> collider, const PhysMaterial &material, bool fixed)
        : collider(std::move(collider)), phys_material(material), fixed(fixed) {
    RecalculateInertia();
}

std::shared_ptr<const Collider> PhysObject::GetCollider() const {
    return std::const_pointer_cast<const Collider>(collider);
}

void PhysObject::Rotate(const Quat &rotation) {
    collider->Rotate(rotation);
    Mat3 rot = GetRotation().Mat();
    inertia.moment_of_inertia_inv_global = rot * inertia.moment_of_inertia_inv * rot.T();
}

void PhysObject::SetRotation(const Quat &rotation) {
    collider->SetRotation(rotation);
    Mat3 rot = GetRotation().Mat();
    inertia.moment_of_inertia_inv_global = rot * inertia.moment_of_inertia_inv * rot.T();
}

const Quat &PhysObject::GetRotation() const {
    return collider->GetRotation();
}

const Vec3 &PhysObject::GetTranslation() const {
    return collider->GetTranslation();
}

void PhysObject::SetTranslation(const Vec3 &translate) {
    collider->SetTranslation(translate);
}

void PhysObject::Translate(const Vec3 &translate) {
    collider->Translate(translate);
}

bool PhysObject::IsSleeping() const {
    return sleeping;
}

int PhysObject::GetId() const {
    return id;
}

void PhysObject::SetId(int id_) {
    id = id_;
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