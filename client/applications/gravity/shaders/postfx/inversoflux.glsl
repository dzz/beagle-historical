#version 330
uniform sampler2D primary_buf;
uniform sampler2D distort_buf;
uniform float factor_a;
uniform float factor_b;
uniform float vx;
uniform float vy;
in vec2 uv;

void main(void) {

    vec4 primary_pixel = texture(primary_buf,uv);
    vec2 uv_t;

    uv_t.x = uv.x+ (factor_a*factor_b) + (vx*0.00001);
    uv_t.y = uv.y+ (factor_a*factor_b) + (vy*0.00001);

    vec4 distorted_pixel = texture(primary_buf,uv_t);

    float l = length(primary_pixel)/3.0;

    if(l<factor_b) {
        gl_FragColor = vec4(l,l,l,1.0);
    } else  if(l>factor_a) {

        gl_FragColor = vec4(0,l,1,1.0);
    }

    {
        gl_FragColor = vec4(l*primary_pixel.b,distorted_pixel.g,l,1.0);
    }
    
}
