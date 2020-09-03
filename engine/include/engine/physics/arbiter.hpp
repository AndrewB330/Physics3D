#pragma once
#include <engine/physics/object.hpp>
#include <engine/physics/constraint.hpp>

/**
 * Arbiter controls all constraints between a pair of bodies
 */
class Arbiter {
public:
    Arbiter();

    Arbiter(PhysObjectPtr a, PhysObjectPtr b);

    void AddConstraint(std::unique_ptr<Constraint> constraint);

    void AddContact(ContactPoint collision);

    /**
     * Initialize all constraints for the current tick.
     * Check if any contact constraint is resolved and delete them.
     */
    void Init();

    /**
     * Solve all constraints
     * @param phase
     */
    void Solve(int phase = 0);

    bool HasConstraints() const;

    bool IsActive() const;

private:
    std::vector<std::shared_ptr<Constraint>> constraints;

    PhysObjectPtr a;
    PhysObjectPtr b;

    bool active = true;
};