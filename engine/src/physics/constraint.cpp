#include <engine/physics/constraints.hpp>
#include <algorithm>

const double PENETRATION_K = 5.0;

CollisionConstraint::CollisionConstraint(std::shared_ptr<PhysObject> a_, std::shared_ptr<PhysObject> b_,
                                         Collision collision)
        : a(std::move(a_)), b(std::move(b_)), penetration(collision.penetration), normal(collision.normal) {

    lever_a = collision.point_a - a->GetPosition();
    lever_b = collision.point_b - b->GetPosition();

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

    double lambda =
            (Dot(relative_velocity + resulting_velocity, normal) + penetration * PENETRATION_K) * effective_mass;

    accumulated_lambda += lambda;
    if (accumulated_lambda < 0.0) {
        accumulated_lambda = 0.0;
        lambda = 0.0;
    }

    a->AddImpulse(+lambda * normal, +lambda * Cross(lever_a, normal));
    b->AddImpulse(-lambda * normal, -lambda * Cross(lever_b, normal));

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

SpringConstraint::SpringConstraint(std::shared_ptr<PhysObject> a, std::shared_ptr<PhysObject> b, Vec3 bind_a,
                                   Vec3 bind_b, double target_length,
                                   double stiffness, double amortizing)
        : a(std::move(a)), b(std::move(b)), target_length(target_length), stiffness(stiffness), amortizing(amortizing),
          bind_a(bind_a),
          bind_b(bind_b) {

}

void SpringConstraint::Solve() {
    if (a->IsFixed() && b->IsFixed()) {
        return;
    }
    // todo: make correct formula

    Vec3 lever_a = a->GetRotation().Mat() * bind_a;
    Vec3 lever_b = b->GetRotation().Mat() * bind_b;

    Vec3 v = (a->GetPosition() + lever_a - b->GetPosition() - lever_b);
    Vec3 direction = v.Norm();
    double length_diff = target_length - v.Len();

    Vec3 point_velocity_a = a->GetAccumulatedVelocity() + Cross(a->GetAccumulatedAngularVelocity(), lever_a);
    Vec3 point_velocity_b = b->GetAccumulatedVelocity() + Cross(b->GetAccumulatedAngularVelocity(), lever_b);
    Vec3 relative_velocity = point_velocity_b - point_velocity_a;

    double lambda = ((Dot(relative_velocity, direction)) + stiffness * length_diff);

    a->AddImpulse(+lambda * direction, +lambda * Cross(lever_a, direction));
    b->AddImpulse(-lambda * direction, -lambda * Cross(lever_b, direction));
}
