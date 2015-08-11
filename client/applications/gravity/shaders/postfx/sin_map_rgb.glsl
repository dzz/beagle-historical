#version 330
uniform sampler2D primary_buf;
uniform sampler2D distort_buf;
uniform float factor_a;
uniform float factor_b;
in vec2 uv;

void main(void) {

    vec4 primary_pixel = texture(primary_buf,uv);

    float l = length(primary_pixel);
    l+=factor_b*(3.14+factor_b);
    float l1=sin(l*3.14);
    float l2=cos(l*3.14*uv.y);
    float l3=sin(l*3.14*uv.x);

    gl_FragColor = vec4(l1,l2,(l1+l2+l3)/3.0,1.0);
    
}
