#include "game_renderer.hpp"

void glVertex(const Vec3 &v) {
    glVertex3f(v.x, v.y, v.z);
}

void DrawVoxelObject(const VoxelObject &object) {
    auto[w, h, d] = object.GetDims();
    glBegin(GL_TRIANGLES);

    // Bottom
    glVertex(Vec3(w, 0, 0));
    glVertex(Vec3(w, 0, d));
    glVertex(Vec3(0, 0, 0));

    glVertex(Vec3(w, 0, d));
    glVertex(Vec3(0, 0, d));
    glVertex(Vec3(0, 0, 0));

    // Top
    glVertex(Vec3(w, h, d));
    glVertex(Vec3(w, h, 0));
    glVertex(Vec3(0, h, 0));

    glVertex(Vec3(0, h, d));
    glVertex(Vec3(w, h, d));
    glVertex(Vec3(0, h, 0));

    // Right
    glVertex(Vec3(w, h, 0));
    glVertex(Vec3(w, h, d));
    glVertex(Vec3(w, 0, 0));

    glVertex(Vec3(w, h, d));
    glVertex(Vec3(w, 0, d));
    glVertex(Vec3(w, 0, 0));

    // Left
    glVertex(Vec3(0, h, d));
    glVertex(Vec3(0, h, 0));
    glVertex(Vec3(0, 0, 0));

    glVertex(Vec3(0, 0, d));
    glVertex(Vec3(0, h, d));
    glVertex(Vec3(0, 0, 0));

    // Front
    glVertex(Vec3(w, h, d));
    glVertex(Vec3(0, h, d));
    glVertex(Vec3(0, 0, d));

    glVertex(Vec3(w, 0, d));
    glVertex(Vec3(w, h, d));
    glVertex(Vec3(0, 0, d));

    // Back
    glVertex(Vec3(0, h, 0));
    glVertex(Vec3(w, h, 0));
    glVertex(Vec3(0, 0, 0));

    glVertex(Vec3(w, h, 0));
    glVertex(Vec3(w, 0, 0));
    glVertex(Vec3(0, 0, 0));

    glEnd();
}

bool isNearOrInside(const Vec3 &position, const VoxelObject &object) {
    Vec3 relPos = object.ApplyTransformInv(position);
    if (relPos.x < -0.5) return false;
    if (relPos.y < -0.5) return false;
    if (relPos.z < -0.5) return false;
    if (relPos.x > object.GetDims().x + 0.5) return false;
    if (relPos.y > object.GetDims().y + 0.5) return false;
    if (relPos.z > object.GetDims().z + 0.5) return false;
    return true;
}

Vec3i getCell(const Vec3 &position, const VoxelObject &object) {
    Vec3 relPos = object.ApplyTransformInv(position);
    return Vec3i(relPos.x, relPos.y, relPos.z);
}

Mat4 GetObjectToWorld(const VoxelObject & object) {
    return Mat4::Transformation(object.GetTranslation(), object.GetRotation(), object.GetScale());
}

Mat4 GetWorldToObject(const VoxelObject & object) {
    return Mat4::TransformationInv(object.GetTranslation(), object.GetRotation(), object.GetScale());
}

bool GameComponent::Init(SDL_Window *window, SDL_GLContext context) {
    sdl_window = window;

    int width, height;
    SDL_GetWindowSize(sdl_window, &width, &height);
    pipeline.Init(width, height);
    Logger::LogInfo("Pipeline initialized");
    textureManager.Init();

    auto &ground = world.GetGroundObject();
    auto &shadow = world.GetShadowObject();
    auto [w, h, d] = ground.GetDims();
    for (int i = 1; i + 1 < w; i++) {
        for (int j = 1; j + 1< h; j++) {
            for (int k = 1; k + 1 < d; k++) {
                /*if (pow(sin(i * 1.0 / ground.GetWidth(0) * 11), 2) +
                    pow(sin(j * 1.0 / ground.GetHeight(0) * 12), 2) +
                    pow(sin(k * 1.0 / ground.GetDepth(0) * 13), 2)
                    < 1.5)
                    continue;*/
                if ((pow(sin((i + k * 0.5) * 1.0 / w * 9), 2) +
                     pow(sin(k * 1.0 / d * 13), 2)) * 32 + 2
                    < j)
                    continue;
                ground.SetVoxelSilent(i, j, k, 1);
            }
        }
    }

    auto obj = std::make_shared<VoxelObject>(32, 48, 32);
    obj->SetRotation(Quat::RandomRotation() * Quat::RandomRotation());
    obj->SetTranslation(Vec3(0, 6.04, 0));
    obj->SetScale(VOXEL_SIZE);

    auto [ww, hh, dd] = obj->GetDims();
    for (int i = 0; i < ww; i++) {
        for (int j = 0; j < hh; j++) {
            for (int k = 0; k < dd; k++) {
                if (pow(i-16,2) + pow(j-16,2)*0.5 + pow(k-16,2) < 14*14) {
                    obj->SetVoxelSilent(i,j,k,1);
                }
            }
        }
    }

    obj->Finalize();
    ground.Finalize();

    world.AddVoxelObject(obj);

    world.GroundUpdated();
    world.ObjectUpdated(obj->GetId());

    textureManager.RegisterVoxelObject(world.GetGroundObject());
    textureManager.RegisterVoxelObject(world.GetShadowObject());
    textureManager.RegisterVoxelObject(*obj);

    pipeline.SetMainFunc([&]() {
        pipeline.SetMainDataTexture(VOXEL_TEXTURE);
        pipeline.SetMainGlobalShadowDims(shadow.GetDims());
        pipeline.SetMainGlobalShadowVoxelSize(shadow.GetScale());
        pipeline.SetMainGlobalShadowMaxLod(shadow.GetFirstFullLod() - 1);
        pipeline.SetMainGlobalShadowLodOffsets(textureManager.GetLodOffsets(shadow.GetId()));
        pipeline.SetMainGlobalShadowTransformInv(GetWorldToObject(shadow));

        auto drawVoxelObject = [&](const VoxelObject &object) {
            pipeline.SetMainCurrentObjectDims(object.GetDims());
            pipeline.SetMainCurrentObjectVoxelSize(object.GetScale());
            pipeline.SetMainCurrentObjectMaxLod(object.GetFirstFullLod() - 1);
            pipeline.SetMainCurrentObjectLodOffsets(textureManager.GetLodOffsets(object.GetId()));
            pipeline.SetMainCurrentObjectDataOffset(textureManager.GetDataOffset(object.GetId()));

            pipeline.SetMainCurrentObjectTransform(GetObjectToWorld(object));

            Vec3 translation = object.GetTranslation();
            Quat rotation = object.GetRotation();

            if (isNearOrInside(camera->GetPosition(), object)) {
                glCullFace(GL_FRONT);
            }

            glPushMatrix();
            glTranslatef(translation.x, translation.y, translation.z);
            glRotatef(acos(std::min(std::max(-1.0, rotation.s), 1.0)) * 2 / PI * 180, rotation.v.x, rotation.v.y, rotation.v.z);
            glScalef(object.GetScale(), object.GetScale(), object.GetScale());
            DrawVoxelObject(object);
            glPopMatrix();

            glCullFace(GL_BACK);
        };
        pipeline.SetMainIndex(0);
        drawVoxelObject(world.GetGroundObject());
        for(const auto & obj : world.GetObjects()) {
            drawVoxelObject(*obj);
        }
        /*pipeline.SetMainIndex(1);
        drawVoxelObject(world.GetShadowObject());*/
    });

    glEnable(GL_CULL_FACE);

    return true;
}

void GameComponent::Redraw() {
    /*auto & obj = world.GetObjects()[0];
    obj->Rotate(Quat::Rotation(0.01, Vec3(0,1,0)));*/

    int width, height;
    SDL_GetWindowSize(sdl_window, &width, &height);
    pipeline.UpdateResolution(width, height);

    if (camera->IsActive() && !oldActive) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
        oldActive = true;
    }
    if (!camera->IsActive() && oldActive) {
        SDL_SetRelativeMouseMode(SDL_FALSE);
        SDL_WarpMouseInWindow(sdl_window, width / 2, height / 2);
        oldActive = false;
    }

    pipeline.CameraTranslation().Get() = camera->GetPosition();
    pipeline.CameraRotation().Get() = camera->GetRotation();


    pipeline.Redraw();
}

void GameComponent::SetCamera(std::shared_ptr<Camera> camera_) {
    camera = std::move(camera_);
}
