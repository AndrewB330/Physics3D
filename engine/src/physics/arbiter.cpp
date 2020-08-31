#include <engine/physics/arbiter.hpp>

Arbiter::Arbiter(std::shared_ptr<PhysObject> a, std::shared_ptr<PhysObject> b) : a(std::move(a)), b(std::move(b)) {}

bool Arbiter::HasConstraints() const {
    return !constraints.empty();
}

void Arbiter::Init(int tick) {

}

void Arbiter::Solve(double dt) {
    for(auto & c : constraints) {
        c->Solve(a, b, dt);
    }
}

void Arbiter::AddConstraint(std::unique_ptr<Constraint> &constraint) {
    if (auto & p = std::dynamic_pointer_cast<CollisionConstraint>(constraint)) {

    }
}
