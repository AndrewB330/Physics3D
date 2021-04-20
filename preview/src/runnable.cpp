#include <application/application.hpp>
#include "debug_ui.hpp"
#include "game_renderer.hpp"
#include "controls/controls.hpp"

// Use Discrete GPU
extern "C" { __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001; }



int main(int argc, char ** argv) {
    Application app;
    app.Init();

    WindowInfo game_window;
    game_window.title = "Voxels Preview";
    game_window.maximized = false;
    game_window.width = 1280;
    game_window.height = 720;

    auto debug_ui = std::make_shared<DebugUI>();
    auto game = std::make_shared<GameComponent>();
    auto controls = std::make_shared<Controls>();
    auto camera = std::make_shared<Camera>(Vec3(0, 16, 0));

    game->SetCamera(camera);

    app.CreateWindow(game_window, {game}, {controls, camera});

    while(app.AnyWindowAlive()) {
        app.PollEvents();
        app.Redraw();
    }

    return 0;
}