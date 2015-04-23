#version 330 core

in vec2 input_position;
in vec2 uv_position;

out vec2 uv;

void main(void) {
    gl_Position.x = input_position.x * 0.1;
    gl_Position.y = input_position.y * 0.1;
    gl_Position.z = 0;
    uv=uv_position;

}
