#pragma once

#include "object.hpp"
#include "constraints.hpp"

class Arbiter {
public:
    Arbiter(std::shared_ptr<PhysObject> a, std::shared_ptr<PhysObject> b);

    void Init(int tick);

    void Solve(double dt);

    void AddConstraint(std::unique_ptr<Constraint> &constraint);

    bool HasConstraints() const;

private:
    std::shared_ptr<PhysObject> a;
    std::shared_ptr<PhysObject> b;

    std::vector<std::unique_ptr<Constraint>> constraints;
};
