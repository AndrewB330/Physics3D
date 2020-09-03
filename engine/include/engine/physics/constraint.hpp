#pragma once

#include "object.hpp"

/**
 * Constraint between two bodies a and b
 */
class Constraint {
public:
    Constraint(PhysObjectPtr a, PhysObjectPtr b);

    /**
     * Call it once per tick, before Solve()
     */
    virtual void Init() = 0;

    /**
     * Solve constraint
     * @param phase - some types of constraints may have several phases
     */
    virtual void Solve(int phase = 0) = 0;

protected:
    PhysObjectPtr a;
    PhysObjectPtr b;
};

/**
 * Describes contact point of two bodies
 * @normal - direction of penetration (from b to a)
 * @local_lever_a - relative position of contact in local coordinates of object a
 * @local_lever_b - relative position of contact in local coordinates of object b
 */
struct ContactPoint {
    Vec3 normal;
    Vec3 local_lever_a;
    Vec3 local_lever_b;
};

/**
 * Prevents two bodies from penetration in point of the contact
 */
class ContactConstraint : public Constraint {
public:
    ContactConstraint(PhysObjectPtr a, PhysObjectPtr b, ContactPoint point);

    void Init() override;

    void Solve(int phase = 0) override;

    /**
     * Check if new contact is similar to the current one, update current contact if so
     * @param contact - new contact point
     * @return true if contact was updated
     */
    bool UpdateIfTheSame(ContactPoint contact);

    /**
     * Check if contact was resolved (or disappeared)
     * @return true iff contact is resolved
     */
    bool IsResolved() const;

    /**
     * Maximum allowed penetration that not cause pushing apart
     */
    const double ALLOWED_PENETRATION = 3e-3;

private:
    /**
     * Compute effective mass of bodies in point of contact by some normal vector
     * @param norm - normal vector
     * @return
     */
    double GetEffectiveMass(const Vec3 & norm) const;

    /**
     * Pseudo phase is when we are pushing bodies apart (using pseudo impulses)
     */
    void SolvePseudoPhase();

    // Original point of contact (absolute coordinates)
    Vec3 initial_point_a;
    Vec3 initial_point_b;

    // Current point of contact (In local body coordinates)
    Vec3 local_lever_a;
    Vec3 local_lever_b;

    // Current point of contact (Relative to the body center)
    Vec3 lever_a;
    Vec3 lever_b;

    // Current point of contact (Absolute coordinates)
    Vec3 point_a;
    Vec3 point_b;

    // normal of contact
    Vec3 normal;

    // pair of perpendicular vectors that form an orthogonal basis with normal (used for friction)
    Vec3 normal_p;
    Vec3 normal_q;

    // penetration depth Dot(point_b-point_a, normal), if less then zero - no penetration
    double penetration;

    // Accumulated impulses (reset each tick)
    double accumulated_lambda; // accumulated impulse along normal axis
    double accumulated_lambda_p; // accumulated impulse along normal_p axis
    double accumulated_lambda_q; // accumulated impulse along normal_q axis
    double accumulated_lambda_pseudo; // accumulated pseudo impulse along normal axis

    // Pre-calculated effective masses along normal, normal_p and normal_q axes
    double effective_mass;
    double effective_mass_p;
    double effective_mass_q;

    // combined friction factor
    double friction_factor;
};