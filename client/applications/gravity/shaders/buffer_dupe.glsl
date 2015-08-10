#version 330

uniform sampler2D buffer;

uniform float time;
uniform vec2 cam;
uniform float scale;
uniform vec4 col1;
uniform vec4 col2;
uniform vec4 col3;
uniform float steps;
uniform float atan_factor;
uniform float circle_factor;
uniform float cosmunge_factor;
uniform float wobble_factor;
uniform float crazy;
uniform float r;


in vec2 uv;

void main(void) {
    vec4 smpl1 = texture(buffer,(uv))*0.5;
    vec4 smpl = texture(buffer,(uv*smpl1.x))*0.5;

    gl_FragColor = (col1*smpl)+(col2*(1-smpl));
}
