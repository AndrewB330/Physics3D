#pragma once

#include <vector>
#include <engine/physics/object.hpp>
#include <memory>
#include <engine/collision/collision_engine.hpp>
#include <engine/physics/constraints.hpp>

struct CollisionRecord {
    PhysObject *a;
    PhysObject *b;
    Collision collision;
};

class World {
public:
    World() = default;

    void Update(double dt);

    std::vector<const PhysObject *> GetObjects() const;

    void SetGravity(Vec3 gravity_);

    void AddObject(std::unique_ptr<PhysObject> object);

    void AddConstraint(std::unique_ptr<Constraint> constraint);

public:
    void Integrate(double dt);

    void GenerateConstraints();

    void InitializeVariables(double dt);

    void SolveConstraints();

    std::vector<std::unique_ptr<Constraint>> constraints;
    std::vector<std::unique_ptr<Constraint>> generated_constraints;
    std::vector<std::shared_ptr<PhysObject>> objects;
    Vec3 gravity;

    CollisionEngine collision_engine;

    double time_elapsed = 0.0;
};