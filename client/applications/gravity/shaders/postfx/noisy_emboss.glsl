#version 330
uniform sampler2D buffer;
uniform sampler2D buffer2;
in vec2 uv;

void main(void) {

    vec4 smpl_base = texture(buffer,uv);
    vec2 uv_t;

    vec4 smpl = texture(buffer2,uv_t);
    uv_t.x = uv.x+ ((smpl.x*0.1));
    uv_t.y = uv.y+ ((smpl.y*0.1));

    vec4 smpl2 = texture(buffer,(uv_t));

    
    gl_FragColor = (smpl+((-1*smpl2)+(1.3*smpl_base)))/2.5;
    //gl_FragColor = smpl_base;
}
