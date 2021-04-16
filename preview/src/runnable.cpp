#include <application/application.hpp>
#include "debug_ui.hpp"
#include "game_renderer.hpp"
#include "controls/controls.hpp"

// Use Discrete GPU
extern "C" { __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001; }



int main(int argc, char ** argv) {
    int a = GL_TEXTURE0;
    int b = GL_TEXTURE5;
    int c = GL_TEXTURE9;
    int d = GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS;
    Application app;
    app.Init();

    WindowInfo game_window;
    game_window.title = "Voxels Preview";

    auto debug_ui = std::make_shared<DebugUI>();
    auto game = std::make_shared<GameComponent>();
    auto controls = std::make_shared<Controls>();
    auto camera = std::make_shared<Camera>();

    game->SetCamera(camera);

    app.CreateWindow(game_window, {game, debug_ui}, {debug_ui, controls, camera});

    while(app.AnyWindowAlive()) {
        app.PollEvents();
        app.Redraw();
    }

    return 0;
}