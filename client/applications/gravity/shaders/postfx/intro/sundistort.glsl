#version 330
uniform sampler2D texBuffer;
uniform sampler2D texDist;
uniform vec4 filter_color;
uniform vec2 uv_translate;
in vec2 uv;

void main(void) {
    vec2 uvT = uv + uv_translate;

    vec4 dp = texture(texDist,uv)*uvT.y;


    vec2 uvT_t0= uv * 1 + (vec2(dp.x,1-dp.y)*0.9);
    vec2 uvT_t1= uvT * 1 + (vec2(1-dp.x,dp.y)*0.9);

    vec4 sb_0 = texture(texBuffer,uvT_t0)*filter_color;
    vec4 sb_1 = texture(texBuffer,uvT_t1);

    vec4 sb_c = vec4(
                        sb_0.r,
                        sb_1.g,
                        sb_0.b,
                        sb_1.a );

    gl_FragColor = (0.5+dp)*texture(texBuffer,uvT);
}
