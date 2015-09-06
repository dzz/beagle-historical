#version 330
uniform sampler2D texBuffer;
uniform sampler2D texDist;
in vec2 uv;

void main(void) {
    vec4 smpl_base = texture(texBuffer,uv) + texture(texDist,uv);
    gl_FragColor = smpl_base;
}
