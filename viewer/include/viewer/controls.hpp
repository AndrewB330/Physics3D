#pragma once

#include <functional>
#include "graphics.hpp"

namespace Controls {
    bool IsKeyPressed(unsigned char key);

    void Init();

    void Update(double dt);

    void LoadTransform();

    void AddKeyboardHandler(std::function<void(unsigned char, int, int)> handler);

    Vec3 GetCameraPosition();

    Vec3 GetCamForward();
}
