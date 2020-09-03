#pragma once
#include <common/math/rand.hpp>

struct PhysMaterial {
public:

    explicit PhysMaterial(double density = 3000, double friction = 0.5, double bounciness = 0.0);

    static PhysMaterial DEFAULT;
    static PhysMaterial WOOD;
    static PhysMaterial STEEL;
    static PhysMaterial ICE;
    static PhysMaterial STONE;
    static PhysMaterial CONCRETE;
    static PhysMaterial RUBBER;

    double GetDensity() const;

    double GetFriction() const;

    double GetBounciness() const;

private:
    double density;
    double friction;
    double bounciness;
};

double CombineFriction(double friction_a, double friction_b);

double CombineBounciness(double bounciness_a, double bounciness_b);
