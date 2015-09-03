#version 330
uniform sampler2D primary_buf;
uniform sampler2D distort_buf;
uniform float frequency;
uniform vec4 color;
in vec2 uv;

void main(void) {

    //vec4 primary_pixel = texture(primary_buf,uv);
    //gl_FragColor = primary_pixel;
    vec4 primary_pixel = texture(primary_buf,uv);

    float l = length(primary_pixel)/2.0;
    l=sin(l*3.14*frequency);

    
    gl_FragColor = (l*color) + ((1-l)*primary_pixel);
    
}
