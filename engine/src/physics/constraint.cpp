#include <engine/physics/constraint.hpp>
#include <common/assert.hpp>

Constraint::Constraint(PhysObjectPtr a, PhysObjectPtr b) : a(std::move(a)), b(std::move(b)) {
}

ContactConstraint::ContactConstraint(PhysObjectPtr a, PhysObjectPtr b, ContactPoint point)
        : Constraint(a, b), local_lever_a(point.local_lever_a), local_lever_b(point.local_lever_b),
          normal(point.normal) {
    friction_factor = CombineFriction(a->GetPhysMaterial().GetFriction(), b->GetPhysMaterial().GetFriction());
    initial_point_a = a->GetTranslation() + a->GetRotation().ApplyRotation(local_lever_a);
    initial_point_b = b->GetTranslation() + b->GetRotation().ApplyRotation(local_lever_b);
}

void ContactConstraint::Init() {
    accumulated_lambda = 0.0;
    accumulated_lambda_p = 0.0;
    accumulated_lambda_q = 0.0;
    accumulated_lambda_pseudo = 0.0;

    lever_a = a->GetRotation().ApplyRotation(local_lever_a);
    lever_b = b->GetRotation().ApplyRotation(local_lever_b);
    point_a = a->GetTranslation() + lever_a;
    point_b = b->GetTranslation() + lever_b;
    penetration = Dot(point_b - point_a, normal);

    if (normal_p.LenSqr() > 1e-3) {
        normal_q = Cross(normal, normal_p).Norm();
        normal_p = Cross(normal_q, normal).Norm();
    } else {
        Vec3 random_vec = Vec3::RandomUnit();
        while (fabs(Cross(random_vec, normal).LenSqr()) < 1e-4) {
            random_vec = Vec3::RandomUnit();
        }
        normal_p = Cross(normal, random_vec).Norm();
        normal_q = Cross(normal, normal_p).Norm();
    }

    // Ensure that normals are perpendicular to each other
    ENSURE_THAT_BETWEEN(normal.Len(), 1.0 - 1e-3, 1.0 + 1e-3)
    ENSURE_THAT_BETWEEN(normal_p.Len(), 1.0 - 1e-3, 1.0 + 1e-3)
    ENSURE_THAT_BETWEEN(normal_q.Len(), 1.0 - 1e-3, 1.0 + 1e-3)
    ENSURE_THAT_BETWEEN(Dot(normal, normal_q), -1e-3, +1e-3)
    ENSURE_THAT_BETWEEN(Dot(normal, normal_p), -1e-3, +1e-3)
    ENSURE_THAT_BETWEEN(Dot(normal_p, normal_q), -1e-3, +1e-3)

    effective_mass = GetEffectiveMass(normal);
    effective_mass_p = GetEffectiveMass(normal_p);
    effective_mass_q = GetEffectiveMass(normal_q);
}

double ContactConstraint::GetEffectiveMass(const Vec3 &norm) const {
    return 1.0 / (
            a->GetInertia().mass_inv +
            b->GetInertia().mass_inv +
            Dot(Cross(a->GetInertia().moment_of_inertia_inv_global * Cross(lever_a, norm), lever_a), norm) +
            Dot(Cross(b->GetInertia().moment_of_inertia_inv_global * Cross(lever_b, norm), lever_b), norm)
    );
}

bool ContactConstraint::IsResolved() const {
    // if there is no penetration anymore or point of contact has shifted
    // todo: magic constant
    return penetration < -1e-2 ||
           Dist(initial_point_a, point_a) > 0.05 ||
           Dist(initial_point_b, point_b) > 0.05 ||
           Dist(point_a, point_b) > fabs(4.0 * penetration) + ALLOWED_PENETRATION;
}

void ContactConstraint::Solve(int phase) {
    if (phase > 0) {
        SolvePseudoPhase();
        return;
    }

    if (penetration < 0.0) {
        return;
    }

    Vec3 point_velocity_a = a->GetAccumulatedVelocity() + Cross(a->GetAccumulatedAngularVelocity(), lever_a);
    Vec3 point_velocity_b = b->GetAccumulatedVelocity() + Cross(b->GetAccumulatedAngularVelocity(), lever_b);
    Vec3 rel_velocity = point_velocity_b - point_velocity_a;

    double lambda = Dot(rel_velocity, normal) * effective_mass;

    accumulated_lambda += lambda;
    if (accumulated_lambda < 0.0) {
        lambda -= accumulated_lambda;
        accumulated_lambda = 0.0;
    }

    a->AddImpulse(+lambda * normal, +lambda * Cross(lever_a, normal));
    b->AddImpulse(-lambda * normal, -lambda * Cross(lever_b, normal));

    // Friction
    // todo: make friction_lambda to be Vec2 (not separate variables)
    double lambda_p = Dot(rel_velocity, normal_p) * effective_mass_p;
    double lambda_q = Dot(rel_velocity, normal_q) * effective_mass_q;

    // todo: change to Vec2
    Vec3 prev_friction = Vec3(accumulated_lambda_p, accumulated_lambda_q, 0);
    Vec3 cur_friction = Vec3(accumulated_lambda_p + lambda_p, accumulated_lambda_q + lambda_q, 0);
    double cur_friction_len = cur_friction.Len();
    if (cur_friction_len > accumulated_lambda * friction_factor) {
        cur_friction = (cur_friction / cur_friction_len) * accumulated_lambda * friction_factor;
    }
    Vec3 delta_friction = cur_friction - prev_friction;
    lambda_p = delta_friction.x;
    lambda_q = delta_friction.y;
    accumulated_lambda_p += lambda_p;
    accumulated_lambda_q += lambda_q;

    a->AddImpulse(+lambda_p * normal_p, +lambda_p * Cross(lever_a, normal_p));
    b->AddImpulse(-lambda_p * normal_p, -lambda_p * Cross(lever_b, normal_p));

    a->AddImpulse(+lambda_q * normal_q, +lambda_q * Cross(lever_a, normal_q));
    b->AddImpulse(-lambda_q * normal_q, -lambda_q * Cross(lever_b, normal_q));
}

void ContactConstraint::SolvePseudoPhase() {
    if (penetration < ALLOWED_PENETRATION) {
        return;
    }

    Vec3 point_velocity_a = a->GetAccumulatedVelocity() + Cross(a->GetAccumulatedAngularVelocity(), lever_a);
    Vec3 point_velocity_b = b->GetAccumulatedVelocity() + Cross(b->GetAccumulatedAngularVelocity(), lever_b);
    Vec3 rel_velocity = point_velocity_b - point_velocity_a;

    double lambda_pseudo = (Dot(rel_velocity, normal) + (penetration - ALLOWED_PENETRATION) * 2.0) * effective_mass;

    accumulated_lambda_pseudo += lambda_pseudo;
    if (accumulated_lambda_pseudo < 0.0) {
        lambda_pseudo -= accumulated_lambda_pseudo;
        accumulated_lambda_pseudo = 0.0;
    }

    a->AddPseudoImpulse(+lambda_pseudo * normal, +lambda_pseudo * Cross(lever_a, normal));
    b->AddPseudoImpulse(-lambda_pseudo * normal, -lambda_pseudo * Cross(lever_b, normal));
}

bool ContactConstraint::UpdateIfTheSame(ContactPoint contact) {
    /**
     * Here are some magic constants.
     * CONTACT_RADIUS - if any new contact is registered within this radius it will be considered as "almost the same"
     * NORMAL_THRESHOLD - the maximum cosine value between two normals to be considered "almost the same"
     */
    const double CONTACT_RADIUS = 0.09;
    const double NORMAL_THRESHOLD = 0.95;
    if (Dist(local_lever_a, contact.local_lever_a) > CONTACT_RADIUS ||
        Dist(local_lever_b, contact.local_lever_b) > CONTACT_RADIUS ||
        Dot(normal, contact.normal) < NORMAL_THRESHOLD) {
        return false;
    }
    /**
     * New contact was too close to the current one -> update current contact with parameters from a new contact
     */
    normal = contact.normal;
    local_lever_a = contact.local_lever_a;
    local_lever_b = contact.local_lever_b;
    initial_point_a = a->GetTranslation() + a->GetRotation().ApplyRotation(local_lever_a);
    initial_point_b = b->GetTranslation() + b->GetRotation().ApplyRotation(local_lever_b);
    return true;
}
