uniform sampler2D buffer;
uniform float time;
uniform vec2 cam;
uniform float scale;
uniform vec4 col1;
uniform vec4 col2;
uniform vec4 col3;
uniform float steps;
uniform float atan_factor;
uniform float circle_factor;
uniform float cosmunge_factor;
uniform float wobble_factor;
uniform float crazy;
uniform float r;


in vec2 uv;

void main(void) {



    float steps = 3;
    float time_t = floor(time*steps)/steps;
    
    if(mod(floor(time_t),2)==0) {
        time_t = time;
    }
    //vec4 col1 = vec4(0,0,0.0,1);
    //vec4 col2 = vec4(1,0,0.5,1);
    //vec4 col3 = vec4(0,0,1,1);
    

    float wobble = (sin(time_t*wobble_factor)+1)/20;
    float cam_scale = 0.01;
    //vec2 tuv = vec2( uv.x * cos(r) - uv.y*sin(r), uv.x * sin(r) + uv.y*cos(r));
    vec2 tuv = uv;

    tuv = tuv * vec2(1,0.5625)*(1+scale);


    tuv=vec2(tuv.x + (cam.x*cam_scale),tuv.y-(cam.y*cam_scale));

    float circle = (tuv.x*tuv.x) + (tuv.y*tuv.y);


    float a=1.0;
    float b=0.0;
    float line =0.0;
    float modfac = atan_factor *0.25;
    circle = (mod( floor(tuv.x*modfac),2)+mod( floor(tuv.y*modfac),2));
    if(circle>1) circle=1;
    a = circle;
    b = 1-circle;



    float c = 0.0;
    
    c = sin(uv.x);
    
    //c = cos(c*circle+time_t+cos(time*cosmunge_factor));
    float d = 1-c;



    
    
     //if(time_t==time){
     //a = floor(a*steps)/steps;
     //c = floor(c*steps)/steps;
     //} else {
     //b = floor(b*steps)/steps;
     //d = floor(d*steps)/steps;
     //}


    gl_FragColor = 
                   ( (((col1*a) + (col2*b) )*d) +
                   (((col2*a) + (col1*b) )*c)   ) +
                   ( (((col1*a) + (col3*b) )*d) +
                   (((col2*a) + (col3*b) )*c)   );

}
