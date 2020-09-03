#pragma once

#include <vector>
#include <engine/physics/object.hpp>
#include <memory>
#include <engine/collision/collision_engine.hpp>
#include <engine/physics/constraint.hpp>
#include <map>
#include "arbiter.hpp"

const int TICKS_PER_UPDATE = 1;
const int SOLVER_LOOPS_PER_TICK = 16;

/**
 * The main class that combines everything together.
 */
class PhysicsEngine {
public:
    PhysicsEngine() = default;

    void Update(double dt);

    std::vector<std::shared_ptr<const PhysObject>> GetObjects() const;

    void SetGravity(Vec3 gravity_);

    int AddObject(std::unique_ptr<PhysObject> object, int id = -1);

    void RemoveObject(int id);

public:
    void Integrate(double dt);

    void GenerateContactPoints();

    void InitializeVariables(double dt);

    void SolveConstraints();

    std::map<std::pair<int, int>, Arbiter> arbiters;
    std::unordered_map<int, PhysObjectPtr> objects;
    int objects_id_generator = 1;

    Vec3 gravity;

    CollisionEngine collision_engine;

    double time_elapsed = 0.0;
    int tick = 0;
};