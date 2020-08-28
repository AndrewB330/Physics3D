#include <engine/physics/constraints.hpp>
#include "../include/engine/world.hpp"
#include "engine/geometry/collision.hpp"

void World::Update(double dt) {
    Integrate(dt);
    DetectCollisions();
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
    objects.push_back(std::move(object));
}

void World::Integrate(double dt) {
    for (auto &o : objects) {
        o->ApplyChanges(dt);
    }
}

void World::DetectCollisions() {
    std::vector<Constraint> cs;
    for(int i = 1; i < objects.size(); i++) {
        for(int j = 0; j < i; j++) {
            CollisionDetector detector(objects[i]->GetShape(), objects[j]->GetShape());
            for(auto c : detector.GetCollisionPoints()) {
                cs.push_back(Constraint(objects[i].get(), objects[j].get(), c));
            }
        }
    }
    for(int i = 0; i < 20; i++) {
        for(auto c : cs) {
            Vec3 impulse = c.GetImpulse();
            c.a->AddImpulse(impulse, c.collision.point);
            c.b->AddImpulse(-impulse, c.collision.point);
        }
    }
}

void World::GenerateConstraints() {}

void World::InitializeVariables(double dt) {
    for (const auto &o : objects) {
        o->AddImpulse(gravity * o->GetInertia().mass * dt, o->GetShape()->GetTranslation());
    }
}

void World::SolveConstraints() {}
