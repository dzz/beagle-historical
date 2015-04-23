#version 330 core

in vec2 input_position;

void main(void) {
    gl_Position.x = input_position.x;
    gl_Position.y = input_position.y;
    gl_Position.z = 0;

}
