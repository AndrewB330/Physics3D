#include "../include/engine/world.hpp"
#include "../include/engine/collision.hpp"

void World::Update(double dt) {
    //Integrate(dt);
    //DetectCollisions();
    //GenerateConstraints();
    //InitializeVariables(dt);
    //SolveConstraints();
    
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->collided = false;
        for (int j = 0; j < i; j++) {
            CollisionDetector c(objects[i], objects[j]);
            if (c.collided) {
                objects[i]->collided = true;
                objects[j]->collided = true;
            }
        }
    }
}

std::vector<const Object*> World::GetObjects() const {
    return std::vector<const Object*>(objects.begin(), objects.end());
}

void World::SetGravity(Vec3 gravity) {
    this->gravity = gravity;
}

void World::AddObject(Object* object) {
    objects.push_back(object);
}

void World::Integrate(double dt) {
    for (auto o : objects) {
        o->ApplyChanges(dt);
    }
}

void World::DetectCollisions() {}

void World::GenerateConstraints() {}

void World::InitializeVariables(double dt) {
    for (auto o : objects) {
        o->AddImpulse(gravity * o->GetInertia().mass * dt, o->GetState().position);
    }
}

void World::SolveConstraints() {}
