#include <iostream>
#include <unordered_map>
#include <functional>
#include <GL/freeglut.h>

#include <math/vec3.hpp>
#include <viewer/controls.hpp>

namespace Controls {

    const double DEFAULT_DT = 1.0 / 60;

    struct Camera {

        double angle_x = 0.0;
        double angle_y = 0.0;

        Vec3 position = Vec3();
        Vec3 velocity = Vec3();
        Vec3 forward = Vec3(0, 0, -1);
        Vec3 up = Vec3(0, 1, 0);

        const double acceleration = 0.5;
        double velocity_fade_factor = 0.95;

        bool cursor = true;
    } camera;

    struct KeyboardState {
        bool pressed[256];
    } keyboard_state;

    struct MouseState {
        int prev_x = 0;
        int prev_y = 0;
    } mouse_state;

    template<typename F>
    class Handlers {
    public:
        int RegisterHandler(F f) {
            functions[id_counter] = f;
            return id_counter++;
        }

        void UnregisterHandler(int id) {
            if (functions.find(id) != functions.end()) {
                functions.erase(functions.find(id));
            }
        }

        std::vector<F> GetFunctions() {
            std::vector<F> funcs;
            for (auto [k, v] : functions) funcs.push_back(v);
            return funcs;
        }

    private:
        std::unordered_map<int, F> functions;
        int id_counter = 0;
    };

    Handlers<std::function<void(int, int, int, int)>> mouse_handlers;
    Handlers<std::function<void(int, int)>> motion_handlers;
    Handlers<std::function<void(int, int)>> passive_motion_handlers;
    Handlers<std::function<void(unsigned char, int, int)>> keyboard_handlers;
    Handlers<std::function<void(unsigned char, int, int)>> keyboard_up_handlers;

    void KeyboardUpFunc(unsigned char key, int x, int y) {
        keyboard_state.pressed[tolower(key)] = false;
        for (auto f : keyboard_up_handlers.GetFunctions()) {
            f(key, x, y);
        }
    }

    void KeyboardFunc(unsigned char key, int x, int y) {
        keyboard_state.pressed[tolower(key)] = true;
        for (auto f : keyboard_handlers.GetFunctions()) {
            f(key, x, y);
        }
    }

    void MouseFunc(int button, int state, int x, int y) {
        for (const auto& f : mouse_handlers.GetFunctions()) {
            f(button, state, x, y);
        }
    }

    void MotionFunc(int x, int y) {
        for (const auto& f : motion_handlers.GetFunctions()) {
            f(x, y);
        }
    }

    void PassiveMotionFunc(int x, int y) {
        for (const auto& f : passive_motion_handlers.GetFunctions()) {
            f(x, y);
        }
    }

    void Update() {
        double mult = camera.acceleration * DEFAULT_DT;
        if (IsKeyPressed('w') || IsKeyPressed('W')) {
            camera.velocity += camera.forward.Norm() * mult;
        }
        if (IsKeyPressed('s') || IsKeyPressed('S')) {
            camera.velocity -= camera.forward.Norm() * mult;
        }
        if (IsKeyPressed('a') || IsKeyPressed('A')) {
            camera.velocity -= Cross(camera.forward, camera.up).Norm() * mult;
        }
        if (IsKeyPressed('d') || IsKeyPressed('D')) {
            camera.velocity += Cross(camera.forward, camera.up).Norm() * mult;
        }
        if (IsKeyPressed('z') || IsKeyPressed('Z')) {
            camera.velocity -= camera.up * mult;
        }
        if (IsKeyPressed(' ')) {
            camera.velocity += camera.up * mult;
        }
        camera.velocity *= camera.velocity_fade_factor;

        camera.position += camera.velocity * DEFAULT_DT;

        if (camera.cursor) {
            glutSetCursor(GLUT_CURSOR_INHERIT);
        } else {
            glutSetCursor(GLUT_CURSOR_NONE);
        }
    }

    void MouseMoved(int x, int y) {
        if (!camera.cursor) {
            int dx = Graphics::GetWidth() / 2 - x;
            int dy = Graphics::GetHeight() / 2 - y;
            double ax = camera.angle_x += dx / 500.0;
            double ay = camera.angle_y += dy / 500.0;
            if (ay > 1.57) ay = 1.57;
            if (ay < -1.57) ay = -1.57;
            camera.forward = Vec3(-cos(ay) * sin(ax), sin(ay), -cos(ay) * cos(ax));
            glutWarpPointer(Graphics::GetWidth() / 2, Graphics::GetHeight() / 2);
        }
    }

    bool IsKeyPressed(unsigned char key) {
        return keyboard_state.pressed[key];
    }

    void Init() {
        glutKeyboardFunc(KeyboardFunc);
        glutKeyboardUpFunc(KeyboardUpFunc);
        glutMouseFunc(MouseFunc);
        glutMotionFunc(MotionFunc);
        glutPassiveMotionFunc(PassiveMotionFunc);

        motion_handlers.RegisterHandler([](int x, int y) {
            MouseMoved(x, y);
        });
        passive_motion_handlers.RegisterHandler([](int x, int y) {
            MouseMoved(x, y);
        });
        keyboard_handlers.RegisterHandler([](unsigned char key, int x, int y) {
            if (key == 27) {
                camera.cursor = true;
            }
        });
        mouse_handlers.RegisterHandler([](int button, int state, int, int) {
            if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
                camera.cursor = false;
                glutWarpPointer(Graphics::GetWidth() / 2, Graphics::GetHeight() / 2);
            }
        });
    }

    void LoadTransform() {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        auto camera_target = camera.position + camera.forward;
        gluLookAt(
            camera.position.x, camera.position.y, camera.position.z,
            camera_target.x, camera_target.y, camera_target.z,
            camera.up.x, camera.up.y, camera.up.z
        );
    }

}
