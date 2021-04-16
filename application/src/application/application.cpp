#include <application/application.hpp>
#include <common/logging.hpp>
#include <GL/glew.h>

bool Application::Init() {
    if (initialized) {
        return true;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Logger::LogError("SDL_Init: %s", SDL_GetError());
        return false;
    } else {
        Logger::LogInfo("SDL Initialized.");
    }
    return initialized = true;
}

Application::~Application() {
    if (initialized) {
        windows.clear();
        Logger::LogInfo("Application destroyed");
        SDL_Quit();
    }
}

bool Application::CreateWindow(WindowInfo window_info,
                               std::vector<std::shared_ptr<WindowRenderer>> renderers,
                               std::vector<std::shared_ptr<WindowListener>> listeners) {
    if (!initialized) {
        return false;
    }
    windows.emplace_back(window_info);
    auto &w = windows.back();
    bool res = w.Init();
    if (!res) {
        windows.pop_back();
    }
    for (auto &r : renderers) {
        w.AddRenderer(std::move(r));
    }
    for (auto &l : listeners) {
        w.AddListener(std::move(l));
    }
    return res;
}

void Application::PollEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        for (auto &window : windows) {
            if (window.ProcessEvent(e)) {
                break;
            }
        }
    }
}

bool Application::AnyWindowAlive() const {
    for (const auto &w : windows) {
        if (!w.IsClosed()) {
            return true;
        }
    }
    return false;
}

void Application::Redraw() {
    for (auto &w : windows) {
        w.Redraw();
    }
}
