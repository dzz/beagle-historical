#version 330
uniform sampler2D texBuffer;
uniform sampler2D blrBuffer;
uniform vec4 filter_color;
in vec2 uv;

void main(void) {
    vec4 smpl_base = texture(texBuffer,uv);
    gl_FragColor = smpl_base;
}
