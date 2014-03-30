#version 330 core

#define band_jitter 0.00390625

precision highp float;
uniform vec4 base_color;
uniform float jitter;

in vec2 uv;
out vec4 color;

float rand(vec2 co){
  return (0.5-fract(cos(dot(co,vec2(4.898,7.23))) * 23421.631));
}

void main(void) {
    float d;
    float cmp_jitter = (jitter+band_jitter);
    vec2 uvn;

    uvn.x = uv.x + (cmp_jitter*rand( vec2(gl_FragCoord.x, gl_FragCoord.y)));
    uvn.y = uv.y + (cmp_jitter*rand( vec2(gl_FragCoord.x+1, gl_FragCoord.y)));
    d = length(uvn) + cmp_jitter*rand( vec2(gl_FragCoord.x+2,gl_FragCoord.y));
    color.r = base_color.r;
    color.g = base_color.g;
    color.b = base_color.b;
    color.a = (1-d) * base_color.a;
}
