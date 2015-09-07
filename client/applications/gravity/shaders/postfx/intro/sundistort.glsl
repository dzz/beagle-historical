#version 330
uniform sampler2D texBuffer;
uniform sampler2D texDist;
uniform vec4 filter_color;
in vec2 uv;

void main(void) {
    vec4 dp = texture(texDist,uv)*uv.y;


    vec2 uv_t0= uv * 1 + (vec2(dp.x,1-dp.y)*0.9);
    vec2 uv_t1= uv * 1 + (vec2(1-dp.x,dp.y)*0.9);

    vec4 sb_0 = texture(texBuffer,uv_t0)*filter_color;
    vec4 sb_1 = texture(texBuffer,uv_t1);

    vec4 sb_c = vec4(
                        sb_0.r,
                        sb_1.g,
                        sb_0.b,
                        sb_1.a );

    gl_FragColor = sb_c * filter_color;
}
