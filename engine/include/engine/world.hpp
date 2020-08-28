#pragma once
#include <vector>
#include <engine/physics/object.hpp>
#include <memory>


class World {
public:
    World() = default;
    
    void Update(double dt);

    std::vector<const PhysObject*> GetObjects() const;

    void SetGravity(Vec3 gravity_);

    void AddObject(std::unique_ptr<PhysObject> object);
public:
    double time_elapsed = 0.0;

    void Integrate(double dt);

    void DetectCollisions();
    void GenerateConstraints();
    void InitializeVariables(double dt);
    void SolveConstraints();

    Vec3 gravity = Vec3(0, -9.8, 0);
    std::vector<std::unique_ptr<PhysObject>> objects;
};