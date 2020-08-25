#pragma once
#include "polyhedron.h"

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