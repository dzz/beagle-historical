#version 330 core

uniform vec4 base_color;

in vec2 uv;
out vec4 color;

void main(void) {

    d = sqrt( (uv.x*uv.x) * (uv.y*uv.y) );

    color.r = base_color.r;
    color.g = base_color.g;
    color.b = base_color.b;
    color.a = d * base_color.a;

}
