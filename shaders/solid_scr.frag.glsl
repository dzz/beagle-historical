#version 330 core

uniform vec3 color;

void main(void) {
    gl_FragColor.r = color.r; 
    gl_FragColor.g = color.g; 
    gl_FragColor.b = color.b; 
    gl_FragColor.a = 1; 
}
