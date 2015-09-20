#version 330
uniform sampler2D texBuffer;
uniform sampler2D modBuffer;
uniform vec4 filter_color;
uniform vec2 term_cam;
uniform float warp;
uniform float time;
in vec2 uv;

void main(void) {

    float nrm_x = (0.0 - term_cam.x) - (2.0*(uv.x-0.5));
    vec2 uv_t = uv;


    if(mod(time*3,10)>=4 && (mod(time*7,90)>60)) {

        uv_t.x+=((sin(warp*((1-uv.y)*(65*warp))*3.14))*(0.01+warp))*0.02;
        uv_t.y+=sin( ((nrm_x*nrm_x)*3.14*20)+(time*2.0f))/256.0;

        uv_t.y*=(0.98)+(0.02-((0.01*(1.0-(nrm_x*nrm_x)))));

        uv_t = (uv_t*0.2)+(uv*0.8);
    }


    float y_idx = uv.y*3.14*20*(1.0+warp);
    float a_warp = ((1.0+sin(y_idx))/2.0);
    float b_warp = 0.9+(0.1*(sin(time+uv.x*10*3.14)));
    float c_warp = 0.9+(0.1*(sin(time+uv.x*20*3.14)));
    float d_warp = 0.9+(0.1*(sin(time+uv.x*30*3.14)));

    vec4 smpl_base_1 = texture(texBuffer,uv_t) * (1.0-(warp*a_warp)) * vec4(1.0*c_warp,1.0*b_warp,1.0*d_warp,1.0-uv.y)*0.6;
    vec4 smpl_base_2 = texture(texBuffer,uv) * (1.0-(warp*a_warp)) * vec4(1.0*c_warp,1.0*b_warp,1.0*d_warp,1.0-uv.y)*0.3;
    vec4 smpl_base_3 = texture(texBuffer,(uv+uv_t)/2.01) * (1.0-(warp*a_warp)) * vec4(1.0*c_warp,1.0*b_warp,1.0*d_warp,1.0-uv.y)*0.3;

    vec4 smpl_base = 
                (smpl_base_1 * vec4(1.0,0.0,0.0,1.0) ) +
                (smpl_base_2 * vec4(0.0,1.0,0.0,1.0) ) +
                (smpl_base_3 * vec4(0.0,0.0,1.0,1.0) );

    vec4 col = (smpl_base  )*smpl_base;

    float mod_color_e = 0.9 + ((sin( uv.x*nrm_x+uv.y*23 ) + cos(a_warp*time)) / 120.0);
    
    float mod_color = (0.75*mod_color_e)+ (0.25*(1.0-(nrm_x*nrm_x)));
    gl_FragColor = col * mod_color + (smpl_base*filter_color) + (0.1*(texture(modBuffer,uv)*(0.5)+(0.2*sin((time*0.3)+y_idx*64))));

}
