#pragma once
#include <string>

namespace Graphics {
    void InitWindow(int& argc, char** argv, const std::string& name, int width, int height, int left = 30, int top = 30);

    void SetRenderFunction(void func());

    void Redraw();

    int GetWidth();
    int GetHeight();
}
