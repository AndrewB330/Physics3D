#pragma once
#include <vector>
#include "../model/object.h"


class World {
public:
    World() = default;
    
    void Update(double dt);

    std::vector<const Object*> GetObjects() const;

    void SetGravity(Vec3 gravity);

    void AddObject(Object* object);
private:

    void Integrate(double dt);

    void DetectCollisions();
    void GenerateConstraints();
    void InitializeVariables(double dt);
    void SolveConstraints();

    Vec3 gravity = Vec3(0, -9.8, 0);
    std::vector<Object*> objects;
};