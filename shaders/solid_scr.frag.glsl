#version 330 core

uniform vec3 color;
in vec2 uv;

void main(void) {
    gl_FragColor.r = color.r*uv.y;
    gl_FragColor.g = color.g*uv.y;
    gl_FragColor.b = color.b*uv.y;
    gl_FragColor.a = 0.5; 
}
