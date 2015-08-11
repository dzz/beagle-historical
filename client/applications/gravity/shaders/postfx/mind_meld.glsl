#version 330
uniform sampler2D primary_buf;
uniform sampler2D distort_buf;
uniform float factor_a;
uniform float factor_b;
in vec2 uv;

void main(void) {

    vec4 primary_pixel = texture(primary_buf,uv);
    vec4 distort_pixel = texture(distort_buf,uv);
    vec2 uv_t;

    uv_t.x = uv.x+ ((distort_pixel.r*0.01*factor_a));
    uv_t.y = uv.y+ ((distort_pixel.g*0.01*factor_b));

    uv_t  = uv_t * (1 + ((distort_pixel.b - 0.5)*factor_a*0.1));

    vec4 distorted_pixel = texture(primary_buf,uv_t);


    if( length(distorted_pixel) < factor_b) {
        gl_FragColor = vec4(1.0,1.0,1.0,1.0);
    } else {
    gl_FragColor = distorted_pixel - (factor_a*(distort_pixel - primary_pixel));
    }
}
