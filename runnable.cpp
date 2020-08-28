#include <iostream>
#include <math/vec3.hpp>
#include <viewer/graphics.hpp>
#include <viewer/controls.hpp>
#include <engine/world.hpp>
#include <engine/timer.hpp>
#include <viewer/drawing.hpp>
using namespace std;

World world;

RenderMaterial material = {
        Color(0.4, 0.3, 0.3),
        Color(0.8, 0.6, 0.4),
        Color(0.1, 0.1, 0.1)
};

void render() {
    Controls::LoadTransform();

    for(auto obj : world.GetObjects()) {
        Graphics::DrawShape(obj->GetShape(), material);
        Graphics::DrawBBox(obj->GetShape()->GetBBox());
    }
}

void init() {
    {
        // Platform
        auto box_shape = std::make_unique<BoxShape>(6.0, 0.5, 6.0);
        auto phys_material = PhysMaterial{5000.0};
        box_shape->SetTranslation(Vec3(0, -2, -3));
        auto phys_object = std::make_unique<PhysObject>(std::move(box_shape), phys_material);
        world.AddObject(std::move(phys_object));
    }
    {
        // Box
        auto box_shape = std::make_unique<BoxShape>(1.0);
        auto phys_material = PhysMaterial{5000.0};
        box_shape->SetTranslation(Vec3(0, +1, -5));
        auto phys_object = std::make_unique<PhysObject>(std::move(box_shape), phys_material);
        phys_object->SetFixed(false);
        phys_object->SetRotation(Quat::Rotation(0.4, Vec3(1, 1, 1)));
        world.AddObject(std::move(phys_object));
    }
    {
        // Box
        auto box_shape = std::make_unique<BoxShape>(1.0);
        auto phys_material = PhysMaterial{5000.0};
        box_shape->SetTranslation(Vec3(-0.1, +2.5, -4.5));
        auto phys_object = std::make_unique<PhysObject>(std::move(box_shape), phys_material);
        phys_object->SetFixed(false);
        phys_object->SetRotation(Quat::Rotation(-0.6, Vec3(1, -1, 1)));
        world.AddObject(std::move(phys_object));
    }

    world.SetGravity(Vec3(0, -3, 0));
}

int main(int argc, char** argv) {
    init();

    Graphics::InitWindow(argc, argv, "Physics3D", 800, 600);
    Controls::Init();

    Graphics::SetRenderFunction(render);

    Timer timer;
    for(int i = 0; i < 1e9; i++) {
        world.Update(timer.GetElapsedSeconds());
        Controls::Update();
        Graphics::Redraw();
    }

    return 0;
}