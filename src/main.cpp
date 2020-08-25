#include <iostream>
#include <vector>
#include "GL\freeglut.h"

#include "math/vec3.h"
#include "global/graphics.h"
#include "global/drawing.h"
#include "global/controls.h"
#include "model/polyhedron.h"
#include "engine/world.h"
#include "model/sphere.h"
#include "model/box.h"

using namespace std;

World world;

void render() {
    Controls::LoadTransform();

    for (auto p : world.GetObjects()) {
        Graphics::DrawObject(p);
    }
}

void init() {
    {
        Box* box = new Box(1, 1, 1);
        Material mat;
        mat.ambient = Color(0.3f, 0.8f, 0.3f);
        mat.diffuse = Color(0.3f, 0.8f, 0.3f);

        box->UpdateMaterial(mat);
        box->UpdateState(Vec3(-2, -1.5, -7), Vec3(), Quat(), Vec3(1, 0, 0));
        box->UpdatePhysicsMaterial({ 5000.0 });
        box->SetFixed(false);

        world.AddObject(box);
    }
    {
        Sphere* sphere = new Sphere(0.8);
        Material mat;
        mat.ambient = Color(0.3f, 0.8f, 0.3f);
        mat.diffuse = Color(0.3f, 0.8f, 0.3f);

        sphere->UpdateMaterial(mat);
        sphere->UpdateState(Vec3(+2, -1.5, -7), Vec3(), Quat(), Vec3());
        sphere->UpdatePhysicsMaterial({ 5000.0 });
        sphere->SetFixed(false);

        world.AddObject(sphere);
    }
    world.SetGravity(Vec3());
}

int main(int argc, char** argv) {
    init();

    Graphics::InitWindow(argc, argv, "Physics3D", 800, 600);
    Controls::Init();

    Graphics::SetRenderFunction(render);

    Timer timer;
    while (true) {
        world.Update(timer.GetElapsedSeconds());
        Controls::Update();
        Graphics::Redraw();
    }

    return 0;
}