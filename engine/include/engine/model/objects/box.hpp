#pragma once
#include "polyhedron.hpp"

class Box : public Polyhedron {
public:
    virtual ~Box() = default;

    Box(double w, double h, double d);

protected:
    double width;
    double height;
    double depth; 
    virtual void RecalculateInertia() override;
};

std::vector<Face> GetBoxFaces();

std::vector<Vec3> GetBoxVertices(double w, double h, double d);

std::vector<Vec3> GetBoxVertices(const BoundingBox & bbox);