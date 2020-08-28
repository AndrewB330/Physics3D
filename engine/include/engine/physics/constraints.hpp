

#include <engine/geometry/collision.hpp>
#include "object.hpp"

class Constraint {
public:
    Constraint(PhysObject *a, PhysObject *b, Collision collision);

    Vec3 GetImpulse() const;

    PhysObject *a;
    PhysObject *b;
    Collision collision;
private:
};
