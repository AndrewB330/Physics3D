#include "camera.hpp"

Camera::Camera(const Vec3 &position) : position(position) {}

void Camera::SetPosition(const Vec3 &position_) {
    position = position_;
}

Vec3 Camera::GetPosition() const {
    return position;
}

Quat Camera::GetRotation() const {
    return Quat::Rotation(roll, Vec3(0, 0, 1)) *
           Quat::Rotation(yaw, Vec3(0, 1, 0)) *
           Quat::Rotation(pitch, Vec3(1, 0, 0));
}

/**
 * Model transformation is inverse camera transformation
 */
Mat4 Camera::GetModelTransform() const {
    return Mat4::TransformationInv(position, GetRotation());
}

Mat4 Camera::GetModelTransformInv() const {
    return Mat4::Transformation(position, GetRotation());
}

bool Camera::ProcessEvent(const SDL_Event &event) {
    if (active && event.type == SDL_MOUSEMOTION) {
        int dx = event.motion.xrel;
        int dy = event.motion.yrel;
        pitch += dy / 500.0;
        yaw -= dx / 500.0;
        return true;
    }
    if (event.type == SDL_MOUSEBUTTONDOWN && !active) {
        active = true;
        // todo: snap cursor
    }
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE && active) {
        active = false;
        // todo: unsnap cursor
    }

    if (event.type == SDL_KEYDOWN) {
        pressed[event.key.keysym.sym] = true;
    }
    if (event.type == SDL_KEYUP) {
        pressed[event.key.keysym.sym] = false;
    }
    return false;
}

void Camera::UpdatePosition() {

    double dt = timer.GetElapsedSinceStep();
    position += velocity * dt;
    if (active) {
        velocity += acceleration_factor * GetRotation().ApplyRotation(
                FORWARD * (pressed['w'] || pressed['W']) +
                BACKWARD * (pressed['s'] || pressed['S']) +
                LEFT * (pressed['a'] || pressed['A']) +
                RIGHT * (pressed['d'] || pressed['D']) +
                UP * (pressed[' ']) +
                DOWN * (pressed['z'] || pressed['Z'])
        ) * dt;
    }
    velocity *= pow(friction_factor, dt);
    timer.Step();
}

void Camera::LookAt(const Vec3 &target) {
    // todo: test this function!!
    Vec3 direction = (target - position).Norm();
    yaw = atan2(direction.x, direction.z);
    double d = sqrt(direction.x * direction.x + direction.z * direction.z);
    pitch = atan2(direction.y, d);
}

void Camera::StartFrameEvent() {
    UpdatePosition();
}

bool Camera::IsActive() const {
    return active;
}
