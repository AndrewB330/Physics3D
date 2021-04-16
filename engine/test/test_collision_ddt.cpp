#include <gtest/gtest.h>
#include <engine/collision/shape.hpp>
#include <engine/collision/collider.hpp>
#include <engine/collision/collision_engine.hpp>
#include "json.hpp"
#include <fstream>

const double DISTANCE_EPSILON = 1e-7;

const double DIRECTION_EPSILON = 1e-7;
const double WEAK_DIRECTION_EPSILON = 1e-4; // for curved shapes (e.g. sphere)

using json = nlohmann::json;

json getData() {
    std::ifstream inp("D:\\Dev\\Cpp\\Physics3D\\cmake-build-release\\data.json");
    json j;
    inp >> j;
    return j;
}

TEST(GJK_DDT, SphereToSphere) {
    auto data = getData();
    for (auto test : data["sphere-to-sphere"]) {
        auto s1 = std::shared_ptr<Collider>(CreateSphereCollider(test["a_radius"].get<float>()));
        auto s2 = std::shared_ptr<Collider>(CreateSphereCollider(test["b_radius"].get<float>()));

        s1->SetTranslation(Vec3(
                test["a_position"][0].get<float>(),
                test["a_position"][1].get<float>(),
                test["a_position"][2].get<float>()
        ));
        s2->SetTranslation(Vec3(
                test["b_position"][0].get<float>(),
                test["b_position"][1].get<float>(),
                test["b_position"][2].get<float>()
        ));

        CollisionEngine eng;

        eng.AddCollider(0, s1);
        eng.AddCollider(1, s2);

        EXPECT_TRUE(eng.GetAllCollisions().empty() != test["collision"].get<bool>());
    }
}

TEST(GJK_DDT, SphereToBox) {
    auto data = getData();
    for (auto test : data["sphere-to-box"]) {
        auto s = std::shared_ptr<Collider>(CreateSphereCollider(test["sphere_radius"].get<float>()));
        auto box = std::shared_ptr<Collider>(CreateBoxCollider(Vec3(
                test["box_shape"][0].get<float>(),
                test["box_shape"][1].get<float>(),
                test["box_shape"][2].get<float>()
        )));

        s->SetTranslation(Vec3(
                test["sphere_position"][0].get<float>(),
                test["sphere_position"][1].get<float>(),
                test["sphere_position"][2].get<float>()
        ));
        box->SetTranslation(Vec3(
                test["box_position"][0].get<float>(),
                test["box_position"][1].get<float>(),
                test["box_position"][2].get<float>()
        ));
        box->SetRotation(Quat(
                test["box_rotation"][0].get<float>(),
                Vec3(test["box_rotation"][1].get<float>(),
                     test["box_rotation"][2].get<float>(),
                     test["box_rotation"][3].get<float>())
        ));

        CollisionEngine eng;

        eng.AddCollider(0, s);
        eng.AddCollider(1, box);

        EXPECT_TRUE(eng.GetAllCollisions().empty() != test["collision"].get<bool>());
    }
}

TEST(GJK_DDT, BoxToBox) {
}