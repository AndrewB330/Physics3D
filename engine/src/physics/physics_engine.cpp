#include <engine/physics/physics_engine.hpp>

void PhysicsEngine::Update(double dt) {
    double tick_dt = dt / TICKS_PER_UPDATE;
    for (int i = 0; i < TICKS_PER_UPDATE; i++) {
        Integrate(tick_dt);
        GenerateContactPoints();
        InitializeVariables(tick_dt);
        SolveConstraints();

        time_elapsed += tick_dt;
        tick++;
    }
}

std::vector<std::shared_ptr<const PhysObject>> PhysicsEngine::GetObjects() const {
    std::vector<std::shared_ptr<const PhysObject>> res;
    for (const auto &[id, o] : objects) {
        res.push_back(std::const_pointer_cast<const PhysObject>(o));
    }
    return res;
}

void PhysicsEngine::SetGravity(Vec3 gravity_) {
    gravity = gravity_;
}

int PhysicsEngine::AddObject(std::unique_ptr<PhysObject> object, int id) {
    if (id < 0) {
        id = objects_id_generator++;
    } else {
        objects_id_generator = std::max(objects_id_generator, id + 1);
    }
    object->SetId(id);

    collision_engine.AddCollider(id, object->GetCollider());
    objects[id] = std::move(object);
    return id;
}

void PhysicsEngine::Integrate(double dt) {
    std::vector<int> to_delete;
    for (auto &[id, o] : objects) {
        o->ApplyChanges(dt);

        // Delete object if far away
        if (o->GetTranslation().Len() > 10000) {
            to_delete.push_back(id);
        }
    }

    for(int id : to_delete) {
        RemoveObject(id);
    }
}

void PhysicsEngine::InitializeVariables(double dt) {
    for (const auto &[id, o] : objects) {
        o->AddImpulse(gravity * o->GetInertia().mass * dt, Vec3());
    }
}

void PhysicsEngine::SolveConstraints() {
    // some ugly logic for awakening some objects that are in contact with not sleeping objects
    std::vector<int> to_awake;
    for (auto &[pair, arbiter] : arbiters) {
        if (!arbiter.HasConstraints()) continue;
        if (objects[pair.first]->IsSleeping() &&
            (!objects[pair.second]->IsSleeping() && !objects[pair.second]->IsFixed())) {
            to_awake.push_back(pair.first);
        }
        if ((!objects[pair.first]->IsSleeping() && !objects[pair.first]->IsFixed()) &&
            objects[pair.second]->IsSleeping()) {
            to_awake.push_back(pair.second);
        }
    }
    for (int id : to_awake) {
        objects[id]->SetSleeping(false);
    }

    // Init all arbiters
    for (auto &[_, arbiter] : arbiters) {
        arbiter.Init();
    }

    // Delete all empty arbiters
    for(auto it = arbiters.begin(); it != arbiters.end();) {
        if (!it->second.HasConstraints()) {
            it = arbiters.erase(it);
        } else {
            ++it;
        }
    }

    // Two phases of solver
    for (int iter = 0; iter < SOLVER_LOOPS_PER_TICK; iter++) {
        for (auto &[_, arbiter] : arbiters) {
            arbiter.Solve(0);
        }
    }

    for (int iter = 0; iter < SOLVER_LOOPS_PER_TICK; iter++) {
        for (auto &[_, arbiter] : arbiters) {
            arbiter.Solve(1);
        }
    }
}

void PhysicsEngine::GenerateContactPoints() {
    for (auto collision : collision_engine.GetAllCollisions()) {
        std::pair<int, int> pair = {collision.a_id, collision.b_id};
        auto &a = objects[collision.a_id];
        auto &b = objects[collision.b_id];
        if ((a->IsFixed() || a->IsSleeping()) && (b->IsFixed() || b->IsSleeping())) {
            continue;
        }
        a->SetSleeping(false);
        b->SetSleeping(false);
        if (arbiters.find(pair) == arbiters.end()) {
            arbiters[pair] = Arbiter(a, b);
        }
        ContactPoint point;
        point.normal = collision.normal;
        point.local_lever_a = a->GetRotation().ApplyInvRotation(collision.point_a - a->GetTranslation());
        point.local_lever_b = b->GetRotation().ApplyInvRotation(collision.point_b - b->GetTranslation());
        arbiters[pair].AddContact(point);
    }
}

void PhysicsEngine::RemoveObject(int id) {
    objects.erase(objects.find(id));
    collision_engine.RemoveCollider(id);

    for(auto it = arbiters.begin(); it != arbiters.end();) {
        if (it->first.first == id || it->first.second == id) {
            it = arbiters.erase(it);
        } else {
            ++it;
        }
    }
}
