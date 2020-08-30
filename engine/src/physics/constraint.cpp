#include <engine/physics/constraints.hpp>
#include <algorithm>

CollisionConstraint::CollisionConstraint(PhysObject *a, PhysObject *b, Collision collision)
        : a(a), b(b), penetration(collision.penetration), normal(collision.normal) {

    lever_a = collision.point - a->GetPosition();
    lever_b = collision.point - b->GetPosition();

    Vec3 angular_mass_a = a->GetInertia().moment_of_inertia_inv_global * Cross(lever_a, normal);
    Vec3 angular_mass_b = b->GetInertia().moment_of_inertia_inv_global * Cross(lever_b, normal);

    effective_mass = 1.0 / (a->GetInertia().mass_inv + Dot(Cross(angular_mass_a, lever_a), normal) +
                            b->GetInertia().mass_inv + Dot(Cross(angular_mass_b, lever_b), normal));


    Vec3 relative_velocity = GetRelativeVelocity();

    tangent_normal = relative_velocity - normal * Dot(relative_velocity, normal);

    if (tangent_normal.Len() > 1e-9) {
        tangent_normal_found = true;
        tangent_normal = tangent_normal.Norm();

        Vec3 tangent_angular_mass_inv_a = a->GetInertia().moment_of_inertia_inv_global * Cross(lever_a, tangent_normal);
        Vec3 tangent_angular_mass_inv_b = b->GetInertia().moment_of_inertia_inv_global * Cross(lever_b, tangent_normal);

        tangent_effective_mass =
                1.0 / (a->GetInertia().mass_inv + Dot(Cross(tangent_angular_mass_inv_a, lever_a), tangent_normal) +
                       b->GetInertia().mass_inv + Dot(Cross(tangent_angular_mass_inv_b, lever_b), tangent_normal));
    } else {
        tangent_normal_found = false;
    }

    // Combined Bounciness
    double bounciness = CombineBounciness(a->GetPhysMaterial().GetBounciness(), b->GetPhysMaterial().GetBounciness());
    resulting_velocity = GetRelativeVelocity() * bounciness;
    // Combined Friction
    friction = CombineFriction(a->GetPhysMaterial().GetFriction(), b->GetPhysMaterial().GetFriction());
}

void CollisionConstraint::Solve() {
    if (a->IsFixed() && b->IsFixed()) {
        return;
    }

    Vec3 relative_velocity = GetRelativeVelocity();

    double lambda = (Dot(relative_velocity + resulting_velocity, normal) + penetration * 5) * effective_mass;

    accumulated_lambda += lambda;
    if (accumulated_lambda < 0.0) {
        accumulated_lambda = 0.0;
        lambda = 0.0;
    }

    a->AddImpulse(+lambda * normal, +lambda * Cross(lever_a, normal));
    b->AddImpulse(-lambda * normal, -lambda * Cross(lever_b, normal));

    /*double k = (a->IsFixed() ? 0.0 : (b->IsFixed() ? 1.0 : 0.5));

    a->AddPseudoVelocity(+0.5 * penetration * normal * k);
    b->AddPseudoVelocity(-0.5 * penetration * normal * (1 - k));*/
    if (tangent_normal_found) {
        double tangentLambda = Dot(relative_velocity, tangent_normal) * tangent_effective_mass;
        tangentLambda = std::min(tangentLambda, lambda * friction);

        a->AddImpulse(+tangentLambda * tangent_normal, +tangentLambda * Cross(lever_a, tangent_normal));
        b->AddImpulse(-tangentLambda * tangent_normal, -tangentLambda * Cross(lever_b, tangent_normal));
    }
}

Vec3 CollisionConstraint::GetRelativeVelocity() const {
    Vec3 point_velocity_a = a->GetAccumulatedVelocity() + Cross(a->GetAccumulatedAngularVelocity(), lever_a);
    Vec3 point_velocity_b = b->GetAccumulatedVelocity() + Cross(b->GetAccumulatedAngularVelocity(), lever_b);
    return point_velocity_b - point_velocity_a;
}

SpringConstraint::SpringConstraint(PhysObject *a, PhysObject *b, Vec3 bind_a, Vec3 bind_b, double target_length,
                                   double stiffness, double amortizing)
        : a(a), b(b), target_length(target_length), stiffness(stiffness), amortizing(amortizing), bind_a(bind_a), bind_b(bind_b) {

}

void SpringConstraint::Solve() {
    if (a->IsFixed() && b->IsFixed()) {
        return;
    }

    Vec3 lever_a = a->GetRotation().Mat() * bind_a;
    Vec3 lever_b = b->GetRotation().Mat() * bind_b;

    Vec3 v = (a->GetPosition() + lever_a - b->GetPosition() - lever_b);
    Vec3 direction = v.Norm();
    double length_diff = target_length - v.Len();

    Vec3 angular_mass_a = a->GetInertia().moment_of_inertia_inv_global * Cross(lever_a, direction);
    Vec3 angular_mass_b = b->GetInertia().moment_of_inertia_inv_global * Cross(lever_b, direction);

    double effective_mass = 1.0 / (a->GetInertia().mass_inv + Dot(Cross(angular_mass_a, lever_a), direction) +
                            b->GetInertia().mass_inv + Dot(Cross(angular_mass_b, lever_b), direction));
    Vec3 point_velocity_a = a->GetAccumulatedVelocity() + Cross(a->GetAccumulatedAngularVelocity(), lever_a);
    Vec3 point_velocity_b = b->GetAccumulatedVelocity() + Cross(b->GetAccumulatedAngularVelocity(), lever_b);
    Vec3 relative_velocity = point_velocity_b - point_velocity_a;

    double lambda = (Dot(relative_velocity, direction)) + stiffness * length_diff;

    a->AddImpulse(+lambda * direction, +lambda * Cross(lever_a, direction));
    b->AddImpulse(-lambda * direction, -lambda * Cross(lever_b, direction));
}
