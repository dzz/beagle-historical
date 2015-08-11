#version 330
uniform sampler2D primary_buf;
uniform sampler2D distort_buf;
uniform float factor_a;
uniform float factor_b;
in vec2 uv;

void main(void) {

    vec4 primary_pixel = texture(primary_buf,uv);

    float l = length(primary_pixel)/2.0;
    l=sin(l*3.14);

    gl_FragColor = vec4(l,l,l,1.0);
    
}
