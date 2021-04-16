#pragma once

#include <application/window_renderer.hpp>
#include "pipeline_gl/pipeline_gl.hpp"
#include "controls/camera.hpp"
#include "game/voxel/voxel_world.hpp"
#include "game/voxel/voxel_texture_manager.hpp"

class GameComponent : public WindowRenderer {
public:
    GameComponent() = default;
    ~GameComponent() override = default;

    bool Init(SDL_Window * window, SDL_GLContext context) override;

    void Redraw() override;

    void SetCamera(std::shared_ptr<Camera> camera);

private:

    SDL_Window * sdl_window = nullptr;
    Pipeline pipeline;

    std::shared_ptr<Camera> camera;

    bool oldActive = false;

    VoxelWorld world;
    VoxelTextureManager textureManager;
};