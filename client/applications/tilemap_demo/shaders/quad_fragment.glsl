#version 330 core

uniform vec4 color;
uniform sampler2D tileset;
in vec2 uv;

void main(void) {
    gl_FragColor = texture(tileset,uv);
}
