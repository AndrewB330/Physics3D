#include <engine/utils.hpp>

std::vector<Triple> GetBoxFaces() {
    return {
            {7, 6, 2},
            {2, 3, 7},
            {0, 4, 5},
            {5, 1, 0},
            {5, 4, 6},
            {6, 7, 5},
            {2, 0, 1},
            {1, 3, 2},
            {7, 3, 1},
            {1, 5, 7},
            {0, 2, 6},
            {6, 4, 0},
    };
}

std::vector<Vec3> GetBoxVertices(double w, double h, double d) {
    return {
            Vec3(-w * 0.5, -h * 0.5, -d * 0.5),
            Vec3(-w * 0.5, -h * 0.5, +d * 0.5),
            Vec3(-w * 0.5, +h * 0.5, -d * 0.5),
            Vec3(-w * 0.5, +h * 0.5, +d * 0.5),
            Vec3(+w * 0.5, -h * 0.5, -d * 0.5),
            Vec3(+w * 0.5, -h * 0.5, +d * 0.5),
            Vec3(+w * 0.5, +h * 0.5, -d * 0.5),
            Vec3(+w * 0.5, +h * 0.5, +d * 0.5),
    };
}

std::vector<Vec3> GetBoxVertices(const BoundingBox &bbox) {
    double w = bbox.max.x - bbox.min.x;
    double h = bbox.max.y - bbox.min.y;
    double d = bbox.max.z - bbox.min.z;
    auto vertices = GetBoxVertices(w, h, d);
    for (auto &v : vertices) {
        v += bbox.min + Vec3(w, h, d) * 0.5;
    }
    return vertices;
}

PhysicsEngine LoadSampleScene() {
    PhysicsEngine world;
    // Build Platform
    double PLATFORM_SIZE = 16.0;
    double BORDER_HEIGHT = 0.5;
    double BORDER_WIDTH = 0.5;
    Vec3 PLATFORM_POS = Vec3(0, -3, -2);
    world.AddObject(std::make_unique<PhysObject>(
            CreateBoxCollider(
                    {PLATFORM_SIZE, 1, PLATFORM_SIZE},
                    PLATFORM_POS
            ),
            PhysMaterial::STONE,
            true
    ));
    world.AddObject(std::make_unique<PhysObject>(
            CreateBoxCollider(
                    {PLATFORM_SIZE, BORDER_HEIGHT, BORDER_WIDTH},
                    PLATFORM_POS + Vec3(0, BORDER_HEIGHT * 0.5 + 0.5, (PLATFORM_SIZE + BORDER_WIDTH) * 0.5)
            ),
            PhysMaterial::STONE,
            true
    ));
    world.AddObject(std::make_unique<PhysObject>(
            CreateBoxCollider(
                    {PLATFORM_SIZE, BORDER_HEIGHT, BORDER_WIDTH},
                    PLATFORM_POS + Vec3(0, BORDER_HEIGHT * 0.5 + 0.5, -(PLATFORM_SIZE + BORDER_WIDTH) * 0.5)
            ),
            PhysMaterial::STONE,
            true
    ));
    world.AddObject(std::make_unique<PhysObject>(
            CreateBoxCollider(
                    {BORDER_WIDTH, BORDER_HEIGHT, PLATFORM_SIZE},
                    PLATFORM_POS + Vec3((PLATFORM_SIZE + BORDER_WIDTH) * 0.5, BORDER_HEIGHT * 0.5 + 0.5, 0)
            ),
            PhysMaterial::STONE,
            true
    ));
    world.AddObject(std::make_unique<PhysObject>(
            CreateBoxCollider(
                    {BORDER_WIDTH, BORDER_HEIGHT, PLATFORM_SIZE},
                    PLATFORM_POS + Vec3(-(PLATFORM_SIZE + BORDER_WIDTH) * 0.5, BORDER_HEIGHT * 0.5 + 0.5, 0)
            ),
            PhysMaterial::STONE,
            true
    ));

    for(int i = 0; i <= 6; i++) {
            for(int j = 0; j <= 12-i; j++) {
                world.AddObject(std::make_unique<PhysObject>(
                        CreateBoxCollider(
                                1.0,
                                PLATFORM_POS + Vec3(i*0.6 + j * 1.2 - 7, i, 0.0)
                        ),
                        PhysMaterial::WOOD,
                        false
                ));
            }
    }

    world.SetGravity(Vec3(0, -9, 0));
    return world;
}
