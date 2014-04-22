#version 330 core

uniform vec4 color0;
uniform vec4 color1;
in vec2 uv;

void main(void) {
    gl_FragColor = ((color0) * uv.y) + ((color1) * (1-uv.y));
}
