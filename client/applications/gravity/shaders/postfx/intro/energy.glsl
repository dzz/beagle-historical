#version 330
uniform sampler2D texBuffer;
uniform vec4 filter_color;
uniform float freq;
in vec2 uv;

void main(void) {
    vec2 uv_t = uv;
    uv_t.x+=(sin(uv.y*freq)*0.001);
    vec4 smpl_base = texture(texBuffer,uv_t);


    float l = (1.0 + sin( (length(smpl_base)/3.0) * freq))/2.0;
    gl_FragColor = vec4(
                        smpl_base.r*l,
                        smpl_base.g*l,
                        smpl_base.b*l,
                        smpl_base.a ) * filter_color;
}
