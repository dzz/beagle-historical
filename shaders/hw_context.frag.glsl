#version 330 core

uniform sampler2D ctxt_sampler;
out vec4 color;
 
void main(void) {
    vec2 sp;
    ivec2 scrp;

    sp.x = gl_FragCoord.x/1920;
    sp.y = gl_FragCoord.y/1080;

    scrp.x = int(gl_FragCoord.x);
    scrp.y = int(gl_FragCoord.y);

    color = texture(ctxt_sampler,sp);
    color = texelFetch(ctxt_sampler,scrp,0);

    //color.a=0.5;
}

