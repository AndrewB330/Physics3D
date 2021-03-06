#include <application/window.hpp>
#include <common/logging.hpp>
#include <GL/glew.h>
#include <common/time_utils.hpp>

Window::~Window() {
    if (sdl_window != nullptr) {
        SDL_DestroyWindow(sdl_window);
        Logger::LogInfo("Window \"%s\" destroyed", info.title.c_str());
    }
}

bool Window::Init() {
    if (initialized) {
        return true;
    }
    sdl_window = SDL_CreateWindow(info.title.c_str(),
                                  SDL_WINDOWPOS_CENTERED_MASK,
                                  SDL_WINDOWPOS_CENTERED_MASK,
                                  info.width,
                                  info.height,
                                  SDL_WINDOW_OPENGL |
                                  SDL_WINDOW_SHOWN |
                                  (info.resizable ? SDL_WINDOW_RESIZABLE : 0) |
                                  (info.maximized ? SDL_WINDOW_MAXIMIZED : 0));
    if (sdl_window == nullptr) {
        Logger::LogInfo("SDL_CreateWindow: %s", SDL_GetError());
        return false;
    } else {
        Logger::LogInfo("[1/3] SDL Window \"%s\" created.", info.title.c_str());
    }
    gl_context = SDL_GL_CreateContext(sdl_window);
    if (gl_context == nullptr) {
        Logger::LogError("SDL_GL_CreateContext: %s", SDL_GetError());
        return false;
    } else {
        Logger::LogInfo("[2/3] SDL GL Context Created.");
    }
    glewInit();
    Logger::LogInfo("[3/3] GLEW Initialized.");
    SDL_GL_SetSwapInterval(0);
    glClearColor(0, 0, 0, 0);
    return initialized = true;
}

Window::Window(WindowInfo info) : info(info) {}

bool Window::ProcessEvent(const SDL_Event &event) {
    if (!initialized || closed) {
        return false;
    }
    int my_id = SDL_GetWindowID(sdl_window);

    if (event.type == SDL_WINDOWEVENT && event.window.windowID == my_id) {
        if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
            SDL_DestroyWindow(sdl_window);
            sdl_window = nullptr;
            closed = true;
            Logger::LogInfo("Window \"%s\" was closed by user", info.title.c_str());
        }
        return true;
    }

    for (auto &l : listeners) {
        if (l->ProcessEvent(event)) {
            return true;
        }
    }

    return false;
}

Window::Window(Window &&window) {
    std::swap(info, window.info);
    std::swap(sdl_window, window.sdl_window);
    std::swap(gl_context, window.gl_context);
    std::swap(initialized, window.initialized);
    std::swap(closed, window.closed);
    std::swap(renderers, window.renderers);
    std::swap(listeners, window.listeners);
}

bool Window::IsClosed() const {
    return closed;
}

void Window::Redraw() {
    if (!initialized || closed) {
        return;
    }

    static FPSMeter fps;
    fps.StartFrame();

    for (auto &l : listeners) {
        l->StartFrameEvent();
    }

    glClear(GL_COLOR_BUFFER_BIT);
    for (auto &r : renderers) {
        r->Redraw();
    }

    SDL_GL_SwapWindow(sdl_window);

    fps.EndFrame();
    static int step = 0;
    if (step % 60 == 0) {
        Logger::LogInfo("FPS: %.02f, SPF: %.02fms", fps.GetFPS(), fps.GetMs());
    }
    step++;
}

void Window::AddRenderer(std::shared_ptr<WindowRenderer> renderer) {
    if (!initialized || closed) {
        return;
    }
    renderer->Init(sdl_window, gl_context);
    renderers.push_back(std::move(renderer));
}

void Window::AddListener(std::shared_ptr<WindowListener> listener) {
    if (!initialized || closed) {
        return;
    }
    listeners.push_back(std::move(listener));
}
