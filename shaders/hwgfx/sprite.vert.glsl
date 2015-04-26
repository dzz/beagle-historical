#version 330 core

//VERTEX SHADER

uniform vec2 view;
uniform float scale_local;
uniform float scale_world;
uniform vec2 translation_local;
uniform vec2 translation_world;

in vec2 input_position;
in vec2 uv_position;

out vec2 uv;

void main(void) {
    gl_Position.x =  (((input_position.x+translation_local.x)*scale_local)+translation_world.x) * scale_world * view.x;
    gl_Position.y =  (((input_position.y+translation_local.y)*scale_local)+translation_world.y) * scale_world * view.y;
    gl_Position.z = 0;
    uv=uv_position;

}
