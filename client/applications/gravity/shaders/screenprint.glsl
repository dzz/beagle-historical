
uniform sampler2D buffer;

uniform float time;
uniform vec2 cam;
uniform float scale;
uniform vec4 col1;
uniform vec4 col2;
uniform vec4 col3;
uniform float steps;
uniform float atan_factor;
uniform float circle_factor;
uniform float cosmunge_factor;
uniform float wobble_factor;
uniform float crazy;
uniform float r;


in vec2 uv;

void main(void) {

    vec2 scam = cam*0.0001;
    vec3 smpl = texture(buffer,(uv*scale) - scam);
    vec2 uv_t;

    uv_t.x = uv.x+smpl.r;
    uv_t.y = uv.y+smpl.g;

    gl_FragColor = (col1*(texture(buffer,(uv* scale) - scam )) * smpl.b) +
                   (col2*(1-texture(buffer,(uv))));
}
