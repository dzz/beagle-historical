#version 330
uniform sampler2D texBuffer;
uniform vec2 uv_translate;
uniform vec2 uv_scale;
uniform vec4 filterColor;
uniform float t;
in vec2 uv;

void main(void) {
    
    float T = t*uv.x;
    vec4 basecol = vec4(0,0.1,0.2,0)*sin(uv.x);
    vec2 uv_t = (uv*uv_scale)+uv_translate;
    if(uv.y>0.15)
        uv_t.y+=sin(T+uv.x)*0.1;
    else {
        basecol = vec4(0,0,0,0);
        uv_t.x+=cos(T+uv.y)*0.1;
    }

    
    vec4 smpl_mod = texture(texBuffer,uv_t) * vec4(sin(uv_t.x*7+T),cos(uv_t.x*9+T),1.0,1.0);
    vec4 smpl_base = texture(texBuffer,uv_t);

    float mod_a = (sin(T)+1.0)/2.0;
    float mod_b = 1-mod_a;
    gl_FragColor =  basecol + ((((smpl_base * filterColor)*mod_a) + (smpl_mod*mod_b))*filterColor);
}
