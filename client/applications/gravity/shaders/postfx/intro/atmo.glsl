#version 330
uniform sampler2D texBuffer;
uniform sampler2D modBuffer;
uniform vec4 color_top;
uniform vec4 color_bot;
uniform float t;
in vec2 uv;

void main(void) {
    vec4 modColor = texture( modBuffer, uv);
    vec2 uv_t = vec2( uv.x + modColor.r, uv.y-modColor.g+(t*0.01) );
    vec4 filterCol = ((uv.y)*color_bot) + ((1-uv.y)*color_top);
    gl_FragColor = (((texture( texBuffer, uv_t )) * filterCol)
                    + filterCol)*filterCol;
}
