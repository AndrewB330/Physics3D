#include "game_renderer.hpp"

void glVertex(const Vec3 &v) {
    glVertex3f(v.x, v.y, v.z);
}

void DrawVoxelObject(const VoxelObject<uint8_t> &object) {
    auto[w, h, d] = object.GetGridDims().ToTuple();
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

bool isNearOrInside(const Vec3 &position, const VoxelObject<uint8_t> &object) {
    Vec3 relPos = object.GetRotation().ApplyInvRotation(position - object.GetTranslation()) / object.GetVoxelSize();
    if (relPos.x < -0.5) return false;
    if (relPos.y < -0.5) return false;
    if (relPos.z < -0.5) return false;
    if (relPos.x > object.GetGridDims().x + 0.5) return false;
    if (relPos.y > object.GetGridDims().y + 0.5) return false;
    if (relPos.z > object.GetGridDims().z + 0.5) return false;
    return true;
}

bool GameComponent::Init(SDL_Window *window, SDL_GLContext context) {
    sdl_window = window;

    int width, height;
    SDL_GetWindowSize(sdl_window, &width, &height);
    pipeline.Init(width, height);
    Logger::LogInfo("Pipeline initialized");
    textureManager.Init();

    auto &ground = world.GetGroundObject();
    for (int i = 0; i < ground.GetWidth(0); i++) {
        for (int j = 0; j < ground.GetHeight(0); j++) {
            for (int k = 0; k < ground.GetDepth(0); k++) {
                /*if (pow(sin(i * 1.0 / ground.GetWidth(0) * 11), 2) +
                    pow(sin(j * 1.0 / ground.GetHeight(0) * 12), 2) +
                    pow(sin(k * 1.0 / ground.GetDepth(0) * 13), 2)
                    < 1.5)
                    continue;*/
                /*if ((pow(sin((i + k * 0.5) * 1.0 / ground.GetWidth(0) * 9), 2) +
                     pow(sin(k * 1.0 / ground.GetDepth(0) * 13), 2)) * 32 + 32
                    < j)
                    continue;*/

                ground.SetVoxelSilent(i, j, k, 1);
            }
        }
    }
    ground.Finalize();

    /*auto obj = std::make_shared<VoxelObject>(WORLD_VOXEL_SIZE, 32, 32, 32);
    for (int i = 0; i < ground.GetWidth(0); i++) {
        for (int j = 0; j < ground.GetHeight(0); j++) {
            for (int k = 0; k < ground.GetDepth(0); k++) {
                if (pow(i-16,2) + pow(j-16,2) + pow(k-16,2) < 16*16) {
                    obj->SetVoxelSilent(i,j,k,1);
                }
            }
        }
    }
    obj->Finalize();
    obj->SetTranslation(Vec3(0, 11, 0));*/

    textureManager.RegisterVoxelObject(world.GetGroundObject());


    //textureManager.RegisterVoxelObject(*obj);

    //world.AddVoxelObject(std::move(obj));

    pipeline.SetMainFunc([&]() {
        static int i = 0;
        i++;
        if (i == 500) {
            textureManager.check();
        }
        pipeline.UpdateUniformMainProjectionMatU();
        pipeline.UpdateUniformMainVoxelsU(VOXEL_TEXTURE);

        auto drawVoxelObject = [&](const VoxelObject<uint8_t> &object) {
            pipeline.UpdateUniformMainGridDimsU(object.GetGridDims());
            pipeline.UpdateUniformMainMaxLodU(object.GetFirstFullLod());
            pipeline.UpdateUniformMainVoxelSizeU(object.GetVoxelSize());
            pipeline.UpdateUniformMainTextureOffsetsU(textureManager.GetTextureOffsets(object.GetId()));

            Vec3 translation = object.GetTranslation();
            Quat rotation = object.GetRotation();

            if (isNearOrInside(camera->GetPosition(), object)) {
                glCullFace(GL_FRONT);
            }

            glPushMatrix();
            glTranslatef(translation.x, translation.y, translation.z);
            glRotatef(acos(std::min(std::max(-1.0, rotation.s), 1.0)) * 2 / PI * 180, rotation.v.x, rotation.v.y, rotation.v.z);
            glScalef(object.GetVoxelSize(), object.GetVoxelSize(), object.GetVoxelSize());
            pipeline.UpdateUniformMainModelViewMatU();
            DrawVoxelObject(object);
            glPopMatrix();

            glCullFace(GL_BACK);
        };

        drawVoxelObject(world.GetGroundObject());
        for(const auto & obj : world.GetObjects()) {
            drawVoxelObject(*obj);
        }

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
        int width, height;
        SDL_GetWindowSize(sdl_window, &width, &height);
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
