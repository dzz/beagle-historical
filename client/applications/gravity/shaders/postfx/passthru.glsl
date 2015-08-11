#version 330
uniform sampler2D buffer;
in vec2 uv;

void main(void) {
    vec4 smpl_base = texture(buffer,uv);
    gl_FragColor = smpl_base;
}
