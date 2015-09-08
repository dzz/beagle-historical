#version 330
uniform sampler2D texBuffer;
in vec2 uv;

void main(void) {
    vec4 smpl_base = texture(texBuffer,uv);
    gl_FragColor = smpl_base;
}
