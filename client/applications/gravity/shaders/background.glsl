#version 330 core

uniform float time;
uniform vec2 cam;
uniform float scale;

in vec2 uv;

void main(void) {

    float steps = 4;
    float time_t = floor(time*steps)/steps;
    
    if(mod(floor(time_t),2)==0) {
        time_t = time;
    }
    vec4 col1 = vec4(0,0,0.0,1);
    vec4 col2 = vec4(1,0,0.5,1);
    vec4 col3 = vec4(0,0,1,1);
    

    float cam_scale = 0.001;
    vec2 tuv = uv * vec2(1,0.5625)*(1+scale);
    tuv=vec2(tuv.x + (cam.x*cam_scale),tuv.y-(cam.y*cam_scale));

    float circle = (tuv.x*tuv.x) + (tuv.y*tuv.y);
    float wobble = (sin(time_t*3.14)+1)/2;


    float a=1.0;
    float b=0.0;
    float line =0.0;
    circle = sin ( circle * 0.2*3.14 )/2.0;
    circle*=circle;
    a = circle;
    b = 1-circle;


    float c = sin((atan(tuv.x,tuv.y)*12)+time_t);
    
    c = cos(c*circle+time_t+cos(time*8));
    b = floor(b*steps)/steps;
    float d = 1-c;



    
    
    if(time_t==time){
    a = floor(a*steps)/steps;
    c = floor(c*steps)/steps;
    } else {
    b = floor(b*steps)/steps;
    d = floor(d*steps)/steps;
    }


    gl_FragColor = 
                   ( (((col1*a) + (col2*b) )*d) +
                   (((col2*a) + (col1*b) )*c)   ) * wobble +
                   ( (((col2*a) + (col3*b) )*d) +
                   (((col3*a) + (col1*b) )*c)   ) * (1-wobble);
}
