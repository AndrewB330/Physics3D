#pragma once
#include <memory>
#include <unordered_map>
#include "collider.hpp"
#include "tree.hpp"

struct Collision {
    int a_id;
    int b_id;
    Vec3 point_a;
    Vec3 point_b;
    Vec3 normal; // from b to a
    double penetration;
};

class CollisionEngine {
public:
    CollisionEngine();

    bool HasColliderId(int id) const;

    void AddCollider(int id, std::shared_ptr<const Collider> collider);

    void RemoveCollider(int id);

    std::vector<Collision> GetAllCollisions();

private:
    std::optional<Collision> GetCollision(int a_id, int b_id) const;

    std::unordered_map<int, std::shared_ptr<const Collider>> colliders;

    Tree tree;
};