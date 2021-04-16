/**
 * This file was auto-generated with OGL_Pipeline generator
 */
#include "pipeline_gl.hpp"

void Pipeline::Init(int width, int height) {
    current_width = std::min(std::max(width, 128), 4096);
    current_height = std::min(std::max(height, 128), 4096);

    glClearColor(0, 0, 0, 1);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    UpdateProjectionMatrix();

    CreateFrameBuffers();
    CreatePrograms();
}

void Pipeline::UpdateResolution(int width, int height) {
    width = std::min(std::max(width, 128), 4096);
    height = std::min(std::max(height, 128), 4096);
    if (width == current_width && height == current_height) {
        return;
    }
    current_width = width;
    current_height = height;
    UpdateProjectionMatrix();

    DestroyFrameBuffers();
    CreateFrameBuffers();
}

void Pipeline::UpdateProjectionMatrix() {
    glViewport(0, 0, current_width, current_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double h = tan(FOV / 360 * PI) * Z_NEAR;
    double w = h * (current_width * 1.0 / current_height);
    glFrustum(-w, w, -h, h, Z_NEAR, Z_FAR);
}

void Pipeline::Redraw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
/*REDRAW*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(180, 0, 1, 0);
    glRotatef(-acos(std::min(std::max(-1.0, camera_rotation.Get().s), 1.0)) * 2 / PI * 180, camera_rotation.Get().v.x, camera_rotation.Get().v.y, camera_rotation.Get().v.z);
    glTranslatef(-camera_translation.Get().x, -camera_translation.Get().y, -camera_translation.Get().z);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glUseProgram(main_p);

    if (main_func) {
        main_func();
    }
    glUseProgram(0);
}

void Pipeline::CreateFrameBuffers() {
/*CREATE_FRAME_BUFFERS*/
}

void Pipeline::CreatePrograms() {
/*CREATE_PROGRAMS*/
    main_p = glCreateProgram();
    CompileAndAttachShader(main_p, "main_vert.glsl", GL_VERTEX_SHADER);
    CompileAndAttachShader(main_p, "main_frag.glsl", GL_FRAGMENT_SHADER);
    glLinkProgram(main_p);

    main_projectionMat_u_inp = glGetUniformLocation(main_p, "projectionMat_u");
    if (main_projectionMat_u_inp == -1) 
        Logger::LogError("Uniform location \"projectionMat_u\" not found");

    main_modelViewMat_u_inp = glGetUniformLocation(main_p, "modelViewMat_u");
    if (main_modelViewMat_u_inp == -1) 
        Logger::LogError("Uniform location \"modelViewMat_u\" not found");

    main_gridDims_u_inp = glGetUniformLocation(main_p, "gridDims_u");
    if (main_gridDims_u_inp == -1) 
        Logger::LogError("Uniform location \"gridDims_u\" not found");

    main_voxels_u_inp = glGetUniformLocation(main_p, "voxels_u");
    if (main_voxels_u_inp == -1) 
        Logger::LogError("Uniform location \"voxels_u\" not found");

    main_maxLod_u_inp = glGetUniformLocation(main_p, "maxLod_u");
    if (main_maxLod_u_inp == -1) 
        Logger::LogError("Uniform location \"maxLod_u\" not found");

    main_voxelSize_u_inp = glGetUniformLocation(main_p, "voxelSize_u");
    if (main_voxelSize_u_inp == -1) 
        Logger::LogError("Uniform location \"voxelSize_u\" not found");

    main_textureOffsets_u_inp = glGetUniformLocation(main_p, "textureOffsets_u[0]");
    if (main_textureOffsets_u_inp == -1) 
        Logger::LogError("Uniform location \"textureOffsets_u\" not found");
}

void Pipeline::DestroyFrameBuffers() {
/*DESTROY_FRAME_BUFFERS*/
}

void Pipeline::CompileAndAttachShader(GLuint program, std::string path, GLenum type) {
    std::ifstream fin(("D:\\Dev\\Cpp\\Physics3D\\preview\\shaders\\" + path).c_str());
    std::stringstream ss;
    ss << fin.rdbuf();
    std::string code_str = ss.str();
    const char *code_c_str = code_str.c_str();
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &code_c_str, NULL);
    glCompileShader(shader);
    glAttachShader(program, shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        Logger::LogError("Shader compilation error %s", infoLog);
    }
}


ValueHolder<Vec3> Pipeline::CameraTranslation() {
    return camera_translation;
}

ValueHolder<Quat> Pipeline::CameraRotation() {
    return camera_rotation;
}

void Pipeline::UniformMat3(GLint location, const Mat3 &mat) {
    float data[9];
    for(int i = 0; i < 9; i++) data[i] = *(*mat.val + i);
    glUniformMatrix3fv(location, 1, GL_TRUE, data);
}

void Pipeline::UniformMat4(GLint location, const Mat4 &mat) {
    float data[16];
    for(int i = 0; i < 16; i++) data[i] = *(*mat.val + i);
    glUniformMatrix4fv(location, 1, GL_TRUE, data);
}

void Pipeline::UniformProjection(GLint location) {
    GLfloat data[16];
    glGetFloatv(GL_PROJECTION_MATRIX, data);
    glUniformMatrix4fv(location, 1, GL_FALSE, data);
}

void Pipeline::UniformModelView(GLint location) {
    GLfloat data[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, data);
    glUniformMatrix4fv(location, 1, GL_FALSE, data);
}

void Pipeline::UniformCameraTranslation(GLint location) {
    Vec3 v = camera_translation.Get();
    glUniform3f(location, v.x, v.y, v.z);
}void Pipeline::SetMainFunc(std::function<void(void)> func) { main_func = std::move(func); }
void Pipeline::UpdateUniformMainProjectionMatU(){ UniformProjection(main_projectionMat_u_inp); }
void Pipeline::UpdateUniformMainModelViewMatU(){ UniformModelView(main_modelViewMat_u_inp); }
void Pipeline::UpdateUniformMainGridDimsU(const Vec3i & value){glUniform3i(main_gridDims_u_inp, value.x, value.y, value.z);}
void Pipeline::UpdateUniformMainVoxelsU(const int & value){ glUniform1i(main_voxels_u_inp, value);}
void Pipeline::UpdateUniformMainMaxLodU(const int & value){ glUniform1i(main_maxLod_u_inp, value);}
void Pipeline::UpdateUniformMainVoxelSizeU(const float & value){glUniform1f(main_voxelSize_u_inp, value);}
void Pipeline::UpdateUniformMainTextureOffsetsU(const std::vector<Vec3i> & value){for (int i = 0; i < value.size(); i++)glUniform3i(main_textureOffsets_u_inp + i, value[i].x, value[i].y, value[i].z);}