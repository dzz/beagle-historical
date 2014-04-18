#version 330 core

uniform vec4 color_a;
uniform vec4 color_b;
in vec2 uv;

void main(void) {
    gl_FragColor = ((color_a) * uv.y) + ((color_b) * (1-uv.y));
}
