#version 330
uniform sampler2D hud_buf;
uniform sampler2D primary_buf;
uniform sampler2D distort_buf;
in vec2 uv;

void main(void) {
    vec4 smpl_base = texture(primary_buf,uv);
    vec4 dist_base = texture(distort_buf,uv);
    gl_FragColor = smpl_base-dist_base;
}
