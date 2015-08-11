#version 330
uniform sampler2D primary_buf;
uniform sampler2D distort_buf;
in vec2 uv;

void main(void) {

    vec4 primary_pixel = texture(primary_buf,uv);
    vec4 distort_pixel = texture(distort_buf,uv);
    gl_FragColor = distort_pixel * primary_pixel;
}
