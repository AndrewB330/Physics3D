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

    main_dataTexture_inp = glGetUniformLocation(main_p, "dataTexture");
    if (main_dataTexture_inp == -1) 
        Logger::LogError("Uniform location \"dataTexture\" not found");

    main_currentObject_dims_inp = glGetUniformLocation(main_p, "currentObject.dims");
    if (main_currentObject_dims_inp == -1) 
        Logger::LogError("Uniform location \"currentObject.dims\" not found");

    main_currentObject_maxLod_inp = glGetUniformLocation(main_p, "currentObject.maxLod");
    if (main_currentObject_maxLod_inp == -1) 
        Logger::LogError("Uniform location \"currentObject.maxLod\" not found");

    main_currentObject_voxelSize_inp = glGetUniformLocation(main_p, "currentObject.voxelSize");
    if (main_currentObject_voxelSize_inp == -1) 
        Logger::LogError("Uniform location \"currentObject.voxelSize\" not found");

    main_currentObject_lodOffsets_inp = glGetUniformLocation(main_p, "currentObject.lodOffsets[0]");
    if (main_currentObject_lodOffsets_inp == -1) 
        Logger::LogError("Uniform location \"currentObject.lodOffsets\" not found");

    main_currentObject_dataOffset_inp = glGetUniformLocation(main_p, "currentObject.dataOffset");
    if (main_currentObject_dataOffset_inp == -1) 
        Logger::LogError("Uniform location \"currentObject.dataOffset\" not found");

    main_currentObject_transform_inp = glGetUniformLocation(main_p, "currentObject.transform");
    if (main_currentObject_transform_inp == -1) 
        Logger::LogError("Uniform location \"currentObject.transform\" not found");

    main_globalShadow_dims_inp = glGetUniformLocation(main_p, "globalShadow.dims");
    if (main_globalShadow_dims_inp == -1) 
        Logger::LogError("Uniform location \"globalShadow.dims\" not found");

    main_globalShadow_maxLod_inp = glGetUniformLocation(main_p, "globalShadow.maxLod");
    if (main_globalShadow_maxLod_inp == -1) 
        Logger::LogError("Uniform location \"globalShadow.maxLod\" not found");

    main_globalShadow_voxelSize_inp = glGetUniformLocation(main_p, "globalShadow.voxelSize");
    if (main_globalShadow_voxelSize_inp == -1) 
        Logger::LogError("Uniform location \"globalShadow.voxelSize\" not found");

    main_globalShadow_lodOffsets_inp = glGetUniformLocation(main_p, "globalShadow.lodOffsets[0]");
    if (main_globalShadow_lodOffsets_inp == -1) 
        Logger::LogError("Uniform location \"globalShadow.lodOffsets\" not found");

    main_globalShadow_transformInv_inp = glGetUniformLocation(main_p, "globalShadow.transformInv");
    if (main_globalShadow_transformInv_inp == -1) 
        Logger::LogError("Uniform location \"globalShadow.transformInv\" not found");

    main_index_inp = glGetUniformLocation(main_p, "index");
    if (main_index_inp == -1) 
        Logger::LogError("Uniform location \"index\" not found");
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
void Pipeline::SetMainFunc(std::function<void(void)> func) { main_func = std::move(func); }
void Pipeline::SetMainDataTexture(const int & value){ glUniform1i(main_dataTexture_inp, value);}
void Pipeline::SetMainCurrentObjectDims(const Vec3i & value){glUniform3i(main_currentObject_dims_inp, value.x, value.y, value.z);}
void Pipeline::SetMainCurrentObjectMaxLod(const int & value){ glUniform1i(main_currentObject_maxLod_inp, value);}
void Pipeline::SetMainCurrentObjectVoxelSize(const float & value){glUniform1f(main_currentObject_voxelSize_inp, value);}
void Pipeline::SetMainCurrentObjectLodOffsets(const std::vector<Vec3i> & value){for (int i = 0; i < value.size(); i++)glUniform3i(main_currentObject_lodOffsets_inp + i, value[i].x, value[i].y, value[i].z);}void Pipeline::SetMainCurrentObjectDataOffset(const Vec3i & value){glUniform3i(main_currentObject_dataOffset_inp, value.x, value.y, value.z);}
void Pipeline::SetMainCurrentObjectTransform(const Mat4 & value){UniformMat4(main_currentObject_transform_inp, value);}
void Pipeline::SetMainGlobalShadowDims(const Vec3i & value){glUniform3i(main_globalShadow_dims_inp, value.x, value.y, value.z);}
void Pipeline::SetMainGlobalShadowMaxLod(const int & value){ glUniform1i(main_globalShadow_maxLod_inp, value);}
void Pipeline::SetMainGlobalShadowVoxelSize(const float & value){glUniform1f(main_globalShadow_voxelSize_inp, value);}
void Pipeline::SetMainGlobalShadowLodOffsets(const std::vector<Vec3i> & value){for (int i = 0; i < value.size(); i++)glUniform3i(main_globalShadow_lodOffsets_inp + i, value[i].x, value[i].y, value[i].z);}void Pipeline::SetMainGlobalShadowTransformInv(const Mat4 & value){UniformMat4(main_globalShadow_transformInv_inp, value);}
void Pipeline::SetMainIndex(const int & value){ glUniform1i(main_index_inp, value);}
