#version 330 core

in vec2 position;
in vec2 uv;

out vec2 uv_out;

void main(void) {
    gl_Position = position;
    uv_out = uv;
}
