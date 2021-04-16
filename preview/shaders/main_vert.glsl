#version 450

layout (location = 0) in vec3 vert_i;

layout (location = 0) out vec3 worldPosition_o;

uniform mat4 projectionMat_u;
uniform mat4 modelViewMat_u;

void main() {
    worldPosition_o = vert_i;

    gl_Position = projectionMat_u * modelViewMat_u * vec4(vert_i, 1);
}
