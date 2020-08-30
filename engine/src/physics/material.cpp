#include <engine/physics/material.hpp>

PhysMaterial PhysMaterial::DEFAULT = PhysMaterial();
PhysMaterial PhysMaterial::WOOD = PhysMaterial(700, 0.45, 0.2);
PhysMaterial PhysMaterial::STEEL = PhysMaterial(7000, 0.7, 0.0);
PhysMaterial PhysMaterial::ICE = PhysMaterial(900, 0.05, 0.0);
PhysMaterial PhysMaterial::STONE = PhysMaterial(3000, 0.95, 0.0);
PhysMaterial PhysMaterial::CONCRETE = PhysMaterial(2400, 0.9, 0.0);
PhysMaterial PhysMaterial::RUBBER = PhysMaterial(1500, 0.7, 0.7);

PhysMaterial::PhysMaterial(double density, double friction, double bounciness)
        : density(density), friction(friction), bounciness(bounciness) {}

double PhysMaterial::GetDensity() const {
    return density;
}

double PhysMaterial::GetFriction() const {
    return friction;
}

double PhysMaterial::GetBounciness() const {
    return bounciness;
}

double CombineFriction(double friction_a, double friction_b) {
    return std::min(friction_a, friction_b);
}

double CombineBounciness(double bounciness_a, double bounciness_b) {
    return std::max(bounciness_a, bounciness_b);
}
