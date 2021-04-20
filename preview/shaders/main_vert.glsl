#version 450

layout (location = 0) in vec3 vert_i;

layout (location = 0) out vec3 worldPosition_o;

uniform mat4 gl_ModelViewMatrix;
uniform mat4 gl_ProjectionMatrix;

void main() {
    worldPosition_o = vert_i;

    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(vert_i, 1);
}
