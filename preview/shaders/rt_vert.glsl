#version 120

varying out vec3 world_position;

void main() {
    world_position = (gl_ModelViewMatrix * gl_Vertex).xyz;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
