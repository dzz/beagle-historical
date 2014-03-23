#version 330 core

uniform sampler2D ctxt_sampler;
out vec4 color;
 
void main(void) {
    vec2 sp;
    ivec2 scrp;

    scrp.x = int(gl_FragCoord.x);
    scrp.y = int(gl_FragCoord.y);

    color = texelFetch(ctxt_sampler,scrp,0);

}

