#version 330
uniform sampler2D texBuffer;
uniform vec4 filter_color;
uniform float freq;
in vec2 uv;

void main(void) {
    vec4 smpl_base = texture(texBuffer,uv);


    float l = (1.0 + sin( (length(smpl_base)/3.0) * freq))/2.0;
    gl_FragColor = vec4(
                        smpl_base.r*l,
                        smpl_base.g*l,
                        smpl_base.b*l,
                        smpl_base.a ) * filter_color;
}
