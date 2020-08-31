#include <engine/world.hpp>

void World::Update(double dt) {
    Integrate(dt);
    GenerateConstraints();
    InitializeVariables(dt);
    SolveConstraints();

    time_elapsed += dt;
}

std::vector<const PhysObject *> World::GetObjects() const {
    std::vector<const PhysObject *> res;
    for (const auto &o : objects) {
        res.push_back(o.get());
    }
    return res;
}

void World::SetGravity(Vec3 gravity_) {
    gravity = gravity_;
}

void World::AddObject(std::unique_ptr<PhysObject> object) {
    collision_engine.AddCollider(objects.size(), object->GetCollider());
    objects.push_back(std::move(object));
}

void World::Integrate(double dt) {
    for (auto &o : objects) {
        o->ApplyChanges(dt);
    }
}

void World::GenerateConstraints() {
    generated_constraints.clear();

    for (auto collision : collision_engine.GetAllCollisions()) {
        generated_constraints.push_back(std::make_unique<CollisionConstraint>(
                objects[collision.a_id],
                objects[collision.b_id],
                collision
        ));
    }
}

void World::InitializeVariables(double dt) {
    for (const auto &o : objects) {
        o->AddImpulse(gravity * o->GetInertia().mass * dt, Vec3());
    }
}

void World::SolveConstraints() {
    for (int iter = 0; iter < 20; iter++) {
        for (const auto &constraint : generated_constraints) {
            constraint->Solve();
        }
        for (const auto &constraint : constraints) {
            constraint->Solve();
        }
    }
}

void World::AddConstraint(std::unique_ptr<Constraint> constraint) {
    constraints.push_back(std::move(constraint));
}
