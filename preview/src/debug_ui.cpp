#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl2.h>
#include <common/logging.hpp>
#include "debug_ui.hpp"

bool DebugUI::Init(SDL_Window *window, SDL_GLContext context) {
    sdl_window = window;
    gl_context = context;
    imgui_context = ImGui::CreateContext();

    IMGUI_CHECKVERSION();
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("consolas.ttf", 14.0f);

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(sdl_window, gl_context);
    ImGui_ImplOpenGL2_Init();

    return true;
}

bool DebugUI::ProcessEvent(const SDL_Event &event) {
    if (event.type == SDL_MOUSEMOTION ||
        event.type == SDL_MOUSEBUTTONDOWN ||
        event.type == SDL_MOUSEBUTTONUP ||
        event.type == SDL_MOUSEWHEEL) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        return ImGui::GetIO().WantCaptureMouse;
    }
    if (event.type == SDL_KEYDOWN ||
        event.type == SDL_KEYUP ||
        event.type == SDL_TEXTINPUT) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        return ImGui::GetIO().WantCaptureKeyboard;
    }
    return false;
}

void DebugUI::Redraw() {
    int width, height;
    SDL_GetWindowSize(sdl_window, &width, &height);

    ImGui::SetCurrentContext(imgui_context);

    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplSDL2_NewFrame(sdl_window);
    ImGui::NewFrame();

    ImGui::SetNextWindowSize({width - 16.f, 0});
    ImGui::SetNextWindowSizeConstraints({width - 16.f, 32}, {width - 16.f, 128});
    ImGui::SetNextWindowPos({8, height - 8.0f}, 0, {0.f, 1.f});
    ImGui::Begin("Logs", nullptr, ImGuiWindowFlags_NoResize);
    for (const auto &s : Logger::Instance().GetLastLogs(32)) {
        if (s.type == LogType::LOG_INFO)
            ImGui::Text("[INFO] %s", s.message.c_str());
        if (s.type == LogType::LOG_WARNING)
            ImGui::Text("[WARNING] %s", s.message.c_str());
        if (s.type == LogType::LOG_ERROR)
            ImGui::Text("[ERROR] %s", s.message.c_str());
    }
    ImGui::SetItemDefaultFocus();
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void DebugUI::StartFrameEvent() {}
