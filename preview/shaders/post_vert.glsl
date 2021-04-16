#version 120

varying out vec2 uv;

void main() {
    gl_Position = gl_Vertex;
    uv = (gl_Vertex.xy + 1) * 0.5;
}
