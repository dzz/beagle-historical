#version 330 core

// x,y  = canvas space
//   z  = dab size
uniform vec3 dab_location

// width/height of canvas 
uniform vec2 scr_size

// this will be a unit -1,1 square
in  vec2 in_Position;
out vec2 uv;

void main(void) {

        gl_Position = vec4( 
                ( ((dab_location.x/scr_size.x) * 2.0) 
                    - 1.0) 
                    * dab_location.z, 
                ( (((scr_size.y - dab_location.y)/scr_size.y) * 2.0) 
                    - 1.0) 
                    * dab_location.z, 
                0.0, 
                1.0);

        uv.x = in_Position.x;
        uv.y = in_Position.y;
}
