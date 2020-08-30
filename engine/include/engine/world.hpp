#pragma once

#include <vector>
#include <engine/physics/object.hpp>
#include <memory>
#include <engine/geometry/collision.hpp>
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

    void DetectCollisions();

    void GenerateConstraints();

    void InitializeVariables(double dt);

    void SolveConstraints();

    std::vector<CollisionRecord> collisions;
    std::vector<std::unique_ptr<Constraint>> constraints;
    std::vector<std::unique_ptr<Constraint>> generated_constraints;
    std::vector<std::unique_ptr<PhysObject>> objects;
    Vec3 gravity;

    double time_elapsed = 0.0;
};