#pragma once

#include <application/window_listener.hpp>
#include "camera.hpp"

class Controls : public WindowListener {
public:
    Controls() = default;

    bool ProcessEvent(const SDL_Event & event) override;

    void StartFrameEvent() override;
private:
};