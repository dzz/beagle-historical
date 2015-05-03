#version 330 core

uniform float time;

in vec2 uv;

void main(void) {

    vec2 t_uv = (uv*2) * 1+(sin(time));

    float wv1 = sin(time/2);
    float wv2 = sin(( 10*wv1 )+t_uv.y+time);
    float wv3 = wv1 + wv2 + sin( uv.y+time/2);
    float x = sin( wv1+wv2+wv3 * 5)+sin(t_uv.x+time);

    gl_FragColor.r = x;
    gl_FragColor.g = wv2;
    gl_FragColor.b = wv1;
    gl_FragColor.a = 1;

}
