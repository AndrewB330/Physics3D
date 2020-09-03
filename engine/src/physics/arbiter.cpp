#include <engine/physics/arbiter.hpp>

Arbiter::Arbiter(PhysObjectPtr a, PhysObjectPtr b) : a(std::move(a)), b(std::move(b)) {}

void Arbiter::AddConstraint(std::unique_ptr<Constraint> constraint) {
    active = true;
    constraints.push_back(std::move(constraint));
}

void Arbiter::AddContact(ContactPoint point) {
    active = true;
    for(const auto & constraint : constraints) {
        if (auto contact_constraint = std::dynamic_pointer_cast<ContactConstraint>(constraint)) {
            if (contact_constraint->UpdateIfTheSame(point)) {
                return;
            }
        }
    }
    constraints.push_back(std::make_shared<ContactConstraint>(a, b, point));
}

void Arbiter::Init() {
    if ((a->IsFixed() || a->IsSleeping()) && (b->IsFixed() || b->IsSleeping()) || constraints.empty()) {
        active = false;
        return;
    }
    std::vector<std::shared_ptr<Constraint>> active_constraints;
    for(const auto & constraint : constraints) {
        constraint->Init();
        if (auto contact_constraint = std::dynamic_pointer_cast<ContactConstraint>(constraint)) {
            if (!contact_constraint->IsResolved()) {
                active_constraints.push_back(std::move(constraint));
            }
        } else {
            active_constraints.push_back(std::move(constraint));
        }
    }
    constraints = active_constraints;
    active = HasConstraints();
}

void Arbiter::Solve(int phase) {
    if (!active) {
        return;
    }
    for(const auto & constraint : constraints) {
        constraint->Solve(phase);
    }
}

Arbiter::Arbiter() {}

bool Arbiter::HasConstraints() const {
    return !constraints.empty();
}

bool Arbiter::IsActive() const {
    return active;
}
