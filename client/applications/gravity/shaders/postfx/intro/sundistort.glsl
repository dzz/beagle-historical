#version 330
uniform sampler2D texBuffer;
uniform sampler2D texDist;
uniform vec4 filter_color;
uniform vec2 uv_translate;
in vec2 uv;

void main(void) {
    vec2 uvT = uv + uv_translate;
    vec4 dp = texture(texDist,uv);
    vec2 dist = vec2( length(dp), 1-length(dp) );

    vec2 uvD = uvT + dist;
    vec4 filtered_orig = texture(texBuffer,uvT) * filter_color;
    vec4 distorted_orig = texture(texBuffer,uvD) * filter_color;


    gl_FragColor = (filtered_orig * dp) + (distorted_orig*(1-dp));
}
