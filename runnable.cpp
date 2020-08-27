#include <iostream>
#include <vector>

#include <math/vec3.hpp>
#include <viewer/graphics.hpp>
#include <viewer/drawing.hpp>
#include <viewer/controls.hpp>
#include <engine/model/objects/polyhedron.hpp>
#include <engine/model/objects/sphere.hpp>
#include <engine/model/objects/box.hpp>
#include <engine/world.hpp>
#include <engine/utils.hpp>

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
        mat.diffuse = Color::GetRandomHsl(0.8, 0.65);
        mat.ambient = mat.diffuse * 0.5;

        box->UpdateMaterial(mat);
        box->UpdateState(Vec3(+0.75, -1.5, -6.0), Vec3(), Quat::Identity(), Vec3(1, 1, 1) * 0);
        box->UpdatePhysicsMaterial({ 5000.0 });
        box->SetFixed(true);

        world.AddObject(box);
    }
    {
        Sphere* sphere = new Sphere(0.8);
        Material mat;
        mat.diffuse = Color::GetRandomHsl(0.8, 0.65);
        mat.ambient = mat.diffuse * 0.5;

        sphere->UpdateMaterial(mat);
        sphere->UpdateState(Vec3(+2, -1.5, -7), Vec3(), Quat::Identity(), Vec3());
        sphere->UpdatePhysicsMaterial({ 5000.0 });
        sphere->SetFixed(true);

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