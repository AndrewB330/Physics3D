#include <iostream>
#include <vector>
#include "GL\freeglut.h"

#include "math/vec3.h"
#include "global/graphics.h"
#include "global/drawing.h"
#include "global/controls.h"
#include "model/polyhedron.h"

using namespace std;

vector<Vec3> vertices = {
    Vec3(-1, -1, -1),
    Vec3(-1, -1, +1),
    Vec3(-1, +1, -1),
    Vec3(-1, +1, +1),
    Vec3(+1, -1, -1),
    Vec3(+1, -1, +1),
    Vec3(+1, +1, -1),
    Vec3(+1, +1, +1),
};

vector<Face> faces = {
    // Top
    Face{7, 6, 2},
    Face{3, 7, 2},
    // Bottom
    Face{4, 5, 0},
    Face{5, 1, 0},
    // Right
    Face{5, 4, 6},
    Face{7, 5, 6},
    // Left
    Face{0, 1, 2},
    Face{1, 3, 2},
    // Front
    Face{7, 3, 1},
    Face{5, 7, 1},
    // Back
    Face{2, 6, 0},
    Face{6, 4, 0},
};

std::vector<Polyhedron> polys = { Polyhedron(vertices, faces) };

void render() {
    Controls::LoadTransform();

    for (const auto& p : polys) {
        Graphics::DrawObject(&p);
    }
}

void init() {
    polys[0].state.position.z = -7;
    polys[0].state.position.x = -2;
    polys[0].state.position.y = -2;
    polys[0].material.ambient = Color(0.3f, 0.8f, 0.3f);
    polys[0].material.diffuse = Color(0.3f, 0.8f, 0.3f);
}

int main(int argc, char** argv) {
    init();

    Graphics::InitWindow(argc, argv, "Physics3D", 800, 600);
    Controls::Init();

    Graphics::SetRenderFunction(render);

    while (true) {
        Controls::Update();
        Graphics::Redraw();
    }

    return 0;
}