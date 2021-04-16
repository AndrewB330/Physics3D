/**
 * This file was auto-generated with OGL_Pipeline generator
 */
#pragma once
#include <functional>
#include <fstream>
#include <sstream>
#include <memory>

#include <GL/glew.h>

#include <math/vec3i.hpp>
#include <math/mat4.hpp>
#include <math/quat.hpp>

#include <common/logging.hpp>

template<typename T>
class ValueHolder {
public:
    ValueHolder() {
        value = std::make_shared<T>();
    }

    static ValueHolder Create() {
        return ValueHolder();
    }

    T & Get() {
        return *value;
    }

    const T & Get() const {
        return *value;
    }

private:
    std::shared_ptr<T> value;
};

class Pipeline {
public:
    void Init(int width, int height);

    void UpdateResolution(int width, int height);

    void Redraw();

    ValueHolder<Vec3> CameraTranslation();

    ValueHolder<Quat> CameraRotation();

/*BIND_QUEUE*/
    void SetMainFunc(std::function<void(void)> func);


/*BIND_VALUE*/
    void UpdateUniformMainProjectionMatU();
    void UpdateUniformMainModelViewMatU();
    void UpdateUniformMainGridDimsU(const Vec3i & value);
    void UpdateUniformMainVoxelsU(const int & value);
    void UpdateUniformMainMaxLodU(const int & value);
    void UpdateUniformMainVoxelSizeU(const float & value);
    void UpdateUniformMainTextureOffsetsU(const std::vector<Vec3i> & value);


private:

    void UpdateProjectionMatrix();

    void CreateFrameBuffers();

    void CreatePrograms();

    void DestroyFrameBuffers();

    void CompileAndAttachShader(GLuint program, std::string path, GLenum type);

    void UniformMat3(GLint location, const Mat3 & mat);

    void UniformMat4(GLint location, const Mat4 & mat);

    void UniformProjection(GLint location);

    void UniformModelView(GLint location);

    void UniformCameraTranslation(GLint location);

    int current_width;
    int current_height;

    ValueHolder<Quat> camera_rotation = ValueHolder<Quat>::Create();
    ValueHolder<Vec3> camera_translation = ValueHolder<Vec3>::Create();

    const double Z_NEAR = 0.01;
    const double Z_FAR = 100.0;
    const double FOV = 90;

    // FrameBuffers(fb) | RenderBuffers(rb) | OutTextures(t)

    // Subprograms(p) | RenderFunctions(func) | Inputs(inp)

    // == main ==
    GLuint main_p;
    std::function<void(void)> main_func = nullptr;
    GLint main_projectionMat_u_inp;
    GLint main_modelViewMat_u_inp;
    GLint main_gridDims_u_inp;
    GLint main_voxels_u_inp;
    GLint main_maxLod_u_inp;
    GLint main_voxelSize_u_inp;
    GLint main_textureOffsets_u_inp;

    // Uniforms(uni)

};
