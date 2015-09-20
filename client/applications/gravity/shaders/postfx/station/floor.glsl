#version 330
uniform sampler2D texBuffer;
uniform sampler2D blrBuffer;
uniform vec4 filter_color;
uniform float px;
in vec2 uv;

void main(void) {
    vec4 smpl_base = texture(texBuffer,uv);


    float mod_x;
    if(mod(uv.y*10,2)>0)
        mod_x = uv.x;
    else 
        mod_x = 0;

    float x = (1.0-uv.y)+(mod((((0.5-mod_x)*(((1.0-uv.y)+0.4)*0.9))*24)+(-1*px),3)/10.0)*0.2;
    x=x*x;
    float s_x=(1.0+sin(x*(0.5-uv.x)*(mod_x+1)*22))/2.0;

    
    gl_FragColor = vec4(uv.y*s_x,0.0,uv.y*s_x*x,1.0);
}
