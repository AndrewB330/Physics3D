#pragma once

#include <application/window_listener.hpp>
#include <math/vec3.hpp>
#include <math/quat.hpp>
#include <math/mat4.hpp>
#include <unordered_map>
#include <common/time_utils.hpp>

class Camera : public WindowListener {
public:
    explicit Camera(const Vec3 &position = Vec3());

    bool ProcessEvent(const SDL_Event &event) override;

    void StartFrameEvent() override;

    void SetPosition(const Vec3 &position);

    Vec3 GetPosition() const;

    Quat GetRotation() const;

    Mat4 GetModelTransform() const;

    Mat4 GetModelTransformInv() const;

    void UpdatePosition();

    void LookAt(const Vec3 &target);

    const Vec3 FORWARD = Vec3(0, 0, 1);
    const Vec3 BACKWARD = Vec3(0, 0, -1);
    const Vec3 LEFT = Vec3(1, 0, 0);
    const Vec3 RIGHT = Vec3(-1, 0, 0);
    const Vec3 UP = Vec3(0, 1, 0);
    const Vec3 DOWN = Vec3(0, -1, 0);

    bool IsActive() const;

private:
    bool active = false;

    Vec3 position = Vec3();
    Vec3 velocity = Vec3();
    double acceleration_factor = 30.0;
    double friction_factor = 0.01;

    std::unordered_map<int, bool> pressed;

    double pitch = PI/2;
    double yaw = 0.0;
    double roll = 0.0;

    Timer timer;
};