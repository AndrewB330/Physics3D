#pragma once

#include <engine/geometry/shape.hpp>
#include <array>
#include <engine/physics/object.hpp>

struct Collision {
    Vec3 point;
    Vec3 normal;
    double penetration = 0.0;
};

class CollisionDetector {
public:
    CollisionDetector(const ConvexShape *a, const ConvexShape *b);

    const std::vector<Collision> &GetCollisionPoints() const;

    bool HasCollision() const;

private:
    std::vector<Collision> collisions;
};
