#version 330

//uniform sampler2D color;

in vec2 uv;

//uniform int iterations;

void main() {
    gl_FragColor = vec4(uv, 0.0, 1);
    //gl_FragColor = texture(color, uv);
}
