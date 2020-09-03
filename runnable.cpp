#include <common/math/vec3.hpp>
#include <viewer/graphics.hpp>
#include <viewer/controls.hpp>
#include <engine/physics/physics_engine.hpp>
#include <common/timer.hpp>
#include <viewer/drawing.hpp>
#include <engine/utils.hpp>
#include <GL/gl.h>

using namespace std;

PhysicsEngine world = LoadSampleScene();

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
    RenderMaterial material;
    material.ambient = material.diffuse = Color(0.99f, 0.2f, 0.1f);

    Controls::LoadTransform();

    auto objs = world.GetObjects();
    for (int i = 0; i < objs.size(); i++) {
        Graphics::DrawCollider(objs[i]->GetCollider().get(), materials[objs[i]->GetId()]);
    }

}

void init() {
    for (int i = 0; i < 1000; i++) {
        RenderMaterial mat;
        mat.diffuse = Color::GetRandomHsl();
        mat.ambient = mat.diffuse * 0.5;
        materials.push_back(mat);
    }
}

int main(int argc, char **argv) {
    init();

    Graphics::InitWindow(argc, argv, "Physics3D", 1200, 800);
    Controls::Init();

    Graphics::SetRenderFunction(render);

    Timer timer;

    Controls::AddKeyboardHandler([](unsigned char c, int, int) {
        if (c == 'q' || c == 'Q') {
            world.AddObject(CreateSphereObject(
                    0.6,
                    PhysMaterial::STEEL,
                    Controls::GetCameraPosition() + Controls::GetCamForward() + Vec3::RandomUnit() * 0,
                    Controls::GetCamForward() * 15
            ));
        }
        if (c == 'e' || c == 'E') {
            //world.Update(1.0/200);
            world.AddObject(CreateBoxObject(
                    Vec3(0.8, 0.8, 0.8),
                    PhysMaterial::STEEL,
                    Controls::GetCameraPosition() + Controls::GetCamForward(),
                    Controls::GetCamForward() * 10,
                    Quat::RandomRotation()
            ));
        }
        std::cout << world.GetObjects().size() << std::endl;
    });

    for (int i = 0; i < 1e9; i++) {
        double dt = timer.GetElapsedSeconds();
        world.Update(dt);

        Controls::Update(dt);
        Graphics::Redraw();
    }

    return 0;
}