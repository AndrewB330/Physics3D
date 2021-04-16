#pragma once

#include <string>
#include <SDL.h>
#include <memory>
#include <vector>
#include <application/window.hpp>

class Application {
public:
    Application() = default;

    ~Application();

    bool Init();

    bool CreateWindow(
            WindowInfo window_info,
            std::vector<std::shared_ptr<WindowRenderer>> renderers = {},
            std::vector<std::shared_ptr<WindowListener>> listeners = {}
    );

    void PollEvents();

    void Redraw();

    bool AnyWindowAlive() const;

private:
    bool initialized = false;

    std::vector<Window> windows;

};
