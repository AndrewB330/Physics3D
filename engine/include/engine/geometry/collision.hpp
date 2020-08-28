#pragma once

#include <array>
#include <engine/geometry/shape.hpp>

const double GJK_MIN_STARTING_VOLUME = 1e-3;
const double EPA_DISTANCE_EPSILON = 1e-8;

struct Collision {
    Vec3 point;
    Vec3 normal;
    double penetration = 0.0;
};

/**
 * Gilbert-Johnson-Keerthi algorithm - method of determining if two convex objects intersect.
 * This implementation can also check if some ConvexShape contains an origin inside.
 */
class GilbertJohnsonKeerthi {
public:
    explicit GilbertJohnsonKeerthi(const ConvexShape *a, const ConvexShape *b);

    explicit GilbertJohnsonKeerthi(const ConvexShape *volume);

    [[nodiscard]] std::array<Vec3, 4> GetSimplex() const;

    [[nodiscard]] bool ContainsOrigin() const;

private:
    void RunAlgorithm(const ConvexShape *shape);

    std::array<Vec3, 4> simplex;
    bool contains_origin = false;
};

class ExpandingPolytopeAlgorithm {
public:
    ExpandingPolytopeAlgorithm(const ConvexShape *shape, std::array<Vec3, 4> starting_simplex);

    ExpandingPolytopeAlgorithm(const ConvexShape *a, const ConvexShape *b, std::array<Vec3, 4> starting_simplex);

    const Vec3 &GetDirection() const;

    double GetDistance() const;

    bool AnswerFound() const;

private:
    void RunAlgorithm(const ConvexShape *shape, std::array<Vec3, 4> starting_simplex);

    Vec3 direction;
    double distance = 0.0;

    bool answer_found = false;
};

class CollisionDetector {
public:
    CollisionDetector(const ConvexShape *a, const ConvexShape *b);

    const std::vector<Collision> &GetCollisionPoints() const;

    bool HasCollision() const;

private:
    std::vector<Collision> collisions;
};
