#version 330
uniform sampler2D texBuffer;
uniform vec2 uv_translate;
uniform vec2 uv_scale;
uniform vec4 filterColor;
uniform float t;
in vec2 uv;

void main(void) {
    
    float T = sin( t+uv.x *3.14 );
    vec4 basecol = vec4(0,0.1,0.2,0)*(0.25+sin(uv.x));
    vec2 uv_t = (uv*uv_scale)+uv_translate;
    if(uv.y>0.0)
        uv_t.y+=uv.x*0.1;
    else {
        basecol = vec4(0,0,0,0);
        uv_t.x+=cos(T+uv.y)*0.1;
    }

    
    vec4 smpl_mod = texture(texBuffer,uv_t) * vec4(sin((uv.x*7)+T)*0.2,cos((uv.x*9)+T)*0.4,1.0,1.0);
    vec4 smpl_base = texture(texBuffer,uv_t);

    float mod_a = (sin(T)+1.0)/2.0;
    float mod_b = 1-mod_a;
    gl_FragColor =  basecol + ((((smpl_base * filterColor)*mod_a) + (smpl_mod*mod_b))*filterColor);
}
