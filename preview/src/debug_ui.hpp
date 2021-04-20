#pragma once

#include <application/window_renderer.hpp>
#include <imgui/imgui.h>
#include <common/time_utils.hpp>

class DebugUI : public WindowRenderer, public WindowListener {
public:
    DebugUI() = default;
    ~DebugUI() override = default;

    bool Init(SDL_Window * window, SDL_GLContext context) override;

    void Redraw() override;

    bool ProcessEvent(const SDL_Event & event) override;

    void StartFrameEvent() override;

private:
    ImGuiContext * imgui_context;

    SDL_Window * sdl_window = nullptr;
    SDL_GLContext gl_context = nullptr;

    FPSMeter fps_meter;
};