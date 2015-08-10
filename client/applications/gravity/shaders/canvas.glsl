#version 330
uniform sampler2D buffer;
uniform sampler2D buffer2;
in vec2 uv;

void main(void) {

    vec4 smpl_base = texture(buffer,uv/2);
    vec2 uv_t;

    vec4 smpl = texture(buffer2,uv_t/2);
    uv_t.x = uv.x+ (sin(smpl.x*0.1)*0.01);
    uv_t.y = uv.y+ (cos(smpl.y*0.1)*0.01);

    vec4 smpl2 = texture(buffer,(uv_t)/2);

    
    gl_FragColor = (smpl+((-1*smpl2)+(1.3*smpl_base)))/2.5;
    if(smpl_base.x<gl_FragColor.x) {
        gl_FragColor=smpl_base;
    }
}
