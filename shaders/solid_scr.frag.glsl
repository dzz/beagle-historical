#version 330 core

uniform vec3 color;
in vec2 uv;

void main(void) {
    float d;
    d = 0.6 + 0.4*(1 - sqrt((uv.x*uv.x)+(uv.y*uv.y)));
    gl_FragColor.r = color.r * d;
    gl_FragColor.g = color.g * d;
    gl_FragColor.b = color.b * d;
    gl_FragColor.a = 1; 
}
