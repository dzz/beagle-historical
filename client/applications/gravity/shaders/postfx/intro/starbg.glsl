#version 330
uniform sampler2D texBuffer;
uniform vec4 filter_color;
uniform vec2 uv_translate;
uniform vec2 uv_scale;
in vec2 uv;

void main(void) {
    
    vec4 rbw = vec4(
                            sin(3*uv.x),
                            sin(2*uv.y+0.2),
                            sin(4*uv.x*uv.y),
                            1.0 );

    vec2 uv_t = (uv*uv_scale) + uv_translate;
    vec4 smpl_base = texture(texBuffer,uv_t);
    gl_FragColor = smpl_base * filter_color * rbw;
}
