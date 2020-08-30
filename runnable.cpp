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

std::vector<RenderMaterial> materials = {
        material,
        material,
        material,
        material,
        material,
};

void render() {
    Controls::LoadTransform();

    auto objs = world.GetObjects();
    for (int i = 0; i < objs.size(); i++) {
        Graphics::DrawShape(objs[i]->GetShape(), materials[i]);
        //Graphics::DrawBBox(objs[i]->GetShape()->GetBBox());
    }
}

void init() {
    for (int i = 0; i < 200; i++) {
        RenderMaterial mat;
        mat.diffuse = Color::GetRandomHsl();
        mat.ambient = mat.diffuse * 0.5;
        materials.push_back(mat);
    }
    {
        // Platform
        auto box_shape = std::make_unique<BoxShape>(8, 1, 8);
        box_shape->SetTranslation(Vec3(0, -4, 0));
        auto phys_object = std::make_unique<PhysObject>(std::move(box_shape), PhysMaterial::STONE);
        world.AddObject(std::move(phys_object));
    }
    {
        // Platform
        auto box_shape = std::make_unique<BoxShape>(8.0, 0.5, 0.5);
        box_shape->SetTranslation(Vec3(0, -3.50, -4.25));
        auto phys_object = std::make_unique<PhysObject>(std::move(box_shape), PhysMaterial::STONE);
        world.AddObject(std::move(phys_object));
    }
    {
        // Platform
        auto box_shape = std::make_unique<BoxShape>(8.0, 0.5, 0.5);
        box_shape->SetTranslation(Vec3(0, -3.50, 4.25));
        auto phys_object = std::make_unique<PhysObject>(std::move(box_shape), PhysMaterial::STONE);
        world.AddObject(std::move(phys_object));
    }
    {
        // Platform
        auto box_shape = std::make_unique<BoxShape>(0.5, 0.5, 8);
        box_shape->SetTranslation(Vec3(-4.25, -3.5, 0));
        auto phys_object = std::make_unique<PhysObject>(std::move(box_shape), PhysMaterial::STONE);
        world.AddObject(std::move(phys_object));
    }
    {
        // Platform
        auto box_shape = std::make_unique<BoxShape>(0.5, 0.5, 8);
        box_shape->SetTranslation(Vec3(4.25, -3.50, 0));
        auto phys_object = std::make_unique<PhysObject>(std::move(box_shape), PhysMaterial::STONE);
        world.AddObject(std::move(phys_object));
    }
    /*{
        double S = 4;
        std::vector<std::unique_ptr<PhysObject>> objects_left;
        std::vector<std::unique_ptr<PhysObject>> objects_right;

        auto anchor_left = std::make_unique<BoxShape>(0.4, 0.4, 0.4);
        anchor_left->SetTranslation(Vec3(-2, 4.0, 0));
        auto anchor_left_object = std::make_unique<PhysObject>(std::move(anchor_left), PhysMaterial::STONE);
        anchor_left_object->SetFixed(true);

        objects_left.push_back(std::move(anchor_left_object));

        auto anchor_right = std::make_unique<BoxShape>(0.4, 0.4, 0.4);
        anchor_right->SetTranslation(Vec3(2, 4.0, 0));
        auto anchor_right_object = std::make_unique<PhysObject>(std::move(anchor_right), PhysMaterial::STONE);
        anchor_right_object->SetFixed(true);

        objects_right.push_back(std::move(anchor_right_object));

        double h = 4.0 - 0.2;

        for(int i = 0; i < 8; i++) {
            auto chain = std::make_unique<BoxShape>(0.1, 0.4, 0.1);
            chain->SetTranslation(Vec3(2, h - 0.3, 0));
            auto chain_object = std::make_unique<PhysObject>(std::move(chain), PhysMaterial::STONE);
            chain_object->SetFixed(false);
            objects_left.push_back(std::move(chain_object));
            h -= 0.5;
        }

        h = 4.0 - 0.2;

        for(int i = 0; i < 8; i++) {
            auto chain = std::make_unique<BoxShape>(0.1, 0.4, 0.1);
            chain->SetTranslation(Vec3(-2, h - 0.3, 0));
            auto chain_object = std::make_unique<PhysObject>(std::move(chain), PhysMaterial::STONE);
            chain_object->SetFixed(false);
            objects_right.push_back(std::move(chain_object));
            h -= 0.5;
        }

        auto ball = std::make_unique<SphereShape>(0.4);
        ball->SetTranslation(Vec3(0, h - 0.5, 0));
        auto ball_object = std::make_unique<PhysObject>(std::move(ball), PhysMaterial::STONE);
        ball_object->SetFixed(false);

        for(int i = 0; i + 1 < objects_left.size(); i++) {
            world.AddConstraint(std::make_unique<SpringConstraint>(
                    objects_left[i].get(),
                    objects_left[i + 1].get(),
                    Vec3(-0.05, -0.2, 0),
                    Vec3(-0.05, 0.2, 0),
                    0.1,
                    S
            ));
            world.AddConstraint(std::make_unique<SpringConstraint>(
                    objects_left[i].get(),
                    objects_left[i + 1].get(),
                    Vec3(+0.05, -0.2, 0),
                    Vec3(+0.05, 0.2, 0),
                    0.1,
                    S
            ));
        }

        for(int i = 0; i + 1 < objects_right.size(); i++) {
            world.AddConstraint(std::make_unique<SpringConstraint>(
                    objects_right[i].get(),
                    objects_right[i + 1].get(),
                    Vec3(-0.05, -0.2, 0),
                    Vec3(-0.05, 0.2, 0),
                    0.1,
                    S
            ));
            world.AddConstraint(std::make_unique<SpringConstraint>(
                    objects_right[i].get(),
                    objects_right[i + 1].get(),
                    Vec3(+0.05, -0.2, 0),
                    Vec3(+0.05, 0.2, 0),
                    0.1,
                    S
            ));
        }

        world.AddConstraint(std::make_unique<SpringConstraint>(
                objects_left.back().get(),
                ball_object.get(),
                Vec3(-0.05, -0.2, 0),
                Vec3(-0.05, 0.4, 0),
                0.1,
                S
        ));

        world.AddConstraint(std::make_unique<SpringConstraint>(
                objects_left.back().get(),
                ball_object.get(),
                Vec3(+0.05, -0.2, 0),
                Vec3(+0.05, 0.4, 0),
                0.1,
                S
        ));

        world.AddConstraint(std::make_unique<SpringConstraint>(
                objects_right.back().get(),
                ball_object.get(),
                Vec3(-0.05, -0.2, 0),
                Vec3(-0.05, 0.4, 0),
                0.1,
                S
        ));

        world.AddConstraint(std::make_unique<SpringConstraint>(
                objects_right.back().get(),
                ball_object.get(),
                Vec3(+0.05, -0.2, 0),
                Vec3(+0.05, 0.4, 0),
                0.1,
                S
        ));

        for(auto & o : objects_left) {
            world.AddObject(std::move(o));
        }
        for(auto & o : objects_right) {
            world.AddObject(std::move(o));
        }
        world.AddObject(std::move(ball_object));
    }*/

    world.SetGravity(Vec3(0, -9, 0));
}

int main(int argc, char **argv) {
    init();

    Graphics::InitWindow(argc, argv, "Physics3D", 1200, 800);
    Controls::Init();

    Graphics::SetRenderFunction(render);

    Timer timer;

    Controls::AddKeyboardHandler([](unsigned char c, int, int) {
        if (c == 'q' || c == 'Q') {
            auto box_shape = std::make_unique<SphereShape>(0.4);
            box_shape->SetTranslation(Controls::GetCameraPosition() + Controls::GetCamForward());
            auto phys_object = std::make_unique<PhysObject>(std::move(box_shape), PhysMaterial::STEEL);
            phys_object->SetFixed(false);
            phys_object->SetVelocity(Controls::GetCamForward() * 10);
            world.AddObject(std::move(phys_object));
        }
        if (c == 'e' || c == 'E') {
            auto box_shape = std::make_unique<BoxShape>(0.8);
            box_shape->SetTranslation(Controls::GetCameraPosition() + Controls::GetCamForward());
            box_shape->SetRotation(Quat::RandomRotation());
            auto phys_object = std::make_unique<PhysObject>(std::move(box_shape), PhysMaterial::WOOD);
            phys_object->SetFixed(false);
            phys_object->SetVelocity(Controls::GetCamForward() * 10);
            world.AddObject(std::move(phys_object));
        }
    });

    for (int i = 0; i < 1e9; i++) {
        world.Update(1 / 1200.0);
        Controls::Update();
        Graphics::Redraw();
    }

    return 0;
}