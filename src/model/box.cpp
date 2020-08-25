#include "box.h"

Box::Box(double w, double h, double d) :
    Polyhedron(
        {
            Vec3(-w * 0.5, -h * 0.5, -d * 0.5),
            Vec3(-w * 0.5, -h * 0.5, +d * 0.5),
            Vec3(-w * 0.5, +h * 0.5, -d * 0.5),
            Vec3(-w * 0.5, +h * 0.5, +d * 0.5),
            Vec3(+w * 0.5, -h * 0.5, -d * 0.5),
            Vec3(+w * 0.5, -h * 0.5, +d * 0.5),
            Vec3(+w * 0.5, +h * 0.5, -d * 0.5),
            Vec3(+w * 0.5, +h * 0.5, +d * 0.5),
        }, {
            Face{7, 6, 2},
            Face{3, 7, 2},
            Face{4, 5, 0},
            Face{5, 1, 0},
            Face{5, 4, 6},
            Face{7, 5, 6},
            Face{0, 1, 2},
            Face{1, 3, 2},
            Face{7, 3, 1},
            Face{5, 7, 1},
            Face{2, 6, 0},
            Face{6, 4, 0},
        }), width(w), height(h), depth(d) {}

void Box::RecalculateInertia() {
    if (fixed) {
        inertia = ObjectInertia();
    } else {
        inertia.mass = width * height * depth * physics_material.density;
        inertia.mass_inv = 1.0 / inertia.mass;

        inertia.moment_of_inertia = Mat33();
        inertia.moment_of_inertia[0][0] = 1 / 12.0 * inertia.mass * (height * height + depth * depth);
        inertia.moment_of_inertia[1][1] = 1 / 12.0 * inertia.mass * (width * width + depth * depth);
        inertia.moment_of_inertia[2][2] = 1 / 12.0 * inertia.mass * (height * height + width * width);

        inertia.moment_of_inertia_inv = inertia.moment_of_inertia.Inverse();
    }
}
