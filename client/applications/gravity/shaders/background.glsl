#version 330 core

uniform float time;
uniform vec2 cam;

in vec2 uv;

void main(void) {

    
    vec4 col1 = vec4(1,1,0.5,1);
    vec4 col2 = vec4(0.3,0.7,1,1);
    vec4 col3 = vec4(1,0.4,1,1);
    

    float cam_scale = 0.01;
    vec2 tuv=vec2(uv.x + (cam.x*cam_scale),uv.y-(cam.y*cam_scale));

    tuv *= vec2(1,0.5625);
    float circle = (tuv.x*tuv.x) + (tuv.y*tuv.y);
    float wobble = (sin(time*3.14)+1)/2;


    float a=1.0;
    float b=0.0;
    float line =0.0;
    float steps = 8;
    circle = sin ( circle * 0.2*3.14 )/2.0;
    circle*=circle;
    a = circle;
    a = floor(a*steps)/steps;
    b = 1-circle;


    float c = sin((atan(tuv.x,tuv.y))+time);
    
    c = cos(c*circle+time);
    c = floor(c*steps)/steps;
    float d = 1-c;



    
    
    //a = floor(a*steps)/steps;
    //b = floor(b*steps)/steps;
    //c = floor(c*steps)/steps;
    //d = floor(d*steps)/steps;


    gl_FragColor = 
                   ( (((col1*a) + (col2*b) )*d) +
                   (((col2*a) + (col1*b) )*c)   ) * wobble +
                   ( (((col2*a) + (col3*b) )*d) +
                   (((col3*a) + (col1*b) )*c)   ) * (1-wobble);
}
