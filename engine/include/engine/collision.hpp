#pragma once
#include <engine/model/object.hpp>

struct Collision {
    Object* a;
    Object* b;

    Vec3 point;
    Vec3 normal;
    double penetration;
};

class CollisionDetector {
public:
    CollisionDetector(Object* a, Object* b);

    const std::vector<Collision>& GetCollisions() const;

    bool HasCollision() const;
    
public:

    bool GilbertJohnsonKeerthi();

    void ExpandingPolytopeAlgorithm();

    void GenerateCollisions();

    Vec3 GetSupportingVector(Vec3 dir);

    bool collided = false;

    Object* a;
    Object* b;
    std::vector<Vec3> vertices;

    std::vector<Collision> collisions;
    
};