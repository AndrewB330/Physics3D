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
    void SetMainDataTexture(const int & value);
    void SetMainCurrentObjectDims(const Vec3i & value);
    void SetMainCurrentObjectMaxLod(const int & value);
    void SetMainCurrentObjectVoxelSize(const float & value);
    void SetMainCurrentObjectLodOffsets(const std::vector<Vec3i> & value);
    void SetMainCurrentObjectDataOffset(const Vec3i & value);
    void SetMainCurrentObjectTransform(const Mat4 & value);
    void SetMainGlobalShadowDims(const Vec3i & value);
    void SetMainGlobalShadowMaxLod(const int & value);
    void SetMainGlobalShadowVoxelSize(const float & value);
    void SetMainGlobalShadowLodOffsets(const std::vector<Vec3i> & value);
    void SetMainGlobalShadowTransformInv(const Mat4 & value);
    void SetMainIndex(const int & value);


private:

    void UpdateProjectionMatrix();

    void CreateFrameBuffers();

    void CreatePrograms();

    void DestroyFrameBuffers();

    void CompileAndAttachShader(GLuint program, std::string path, GLenum type);

    void UniformMat3(GLint location, const Mat3 & mat);

    void UniformMat4(GLint location, const Mat4 & mat);

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
    GLint main_dataTexture_inp;
    GLint main_currentObject_dims_inp;
    GLint main_currentObject_maxLod_inp;
    GLint main_currentObject_voxelSize_inp;
    GLint main_currentObject_lodOffsets_inp;
    GLint main_currentObject_dataOffset_inp;
    GLint main_currentObject_transform_inp;
    GLint main_globalShadow_dims_inp;
    GLint main_globalShadow_maxLod_inp;
    GLint main_globalShadow_voxelSize_inp;
    GLint main_globalShadow_lodOffsets_inp;
    GLint main_globalShadow_transformInv_inp;
    GLint main_index_inp;

    // Uniforms(uni)

};
