#shader

```python
import client.gfx.shaders as shaders
```

a shader represents a GLSL language text file, which has been loaded to the video driver and compiled. The first time you use a shader, it is lazy compiled and uploaded to the gpu. In order to use a particular shader configuration, you access the shaders module static method to get a handle to that particular GPU object. 

Upon calling the *bind* method to specify the shader uniform inputs ( if you ar unsure of what this refers to, please research OpenGL 3 shader pipeline ), the shader program is selected on the GPU and will be used for all subsequent draw calls.

vertex shaders are assumed to be stores as "{name}.vert.glsl" and fragment shaders are assumed to be "{name}.frag.glsl".

The are stored in the shaders directory from the root of the repo, or where the ctt2 binary is invoked.

#usage

```python

import hwgfx
import client.gfx.shaders as shaders

#this will load and compile the shader if it hasn't
#be compiled and linked on the gpu already

shader = shaders.get(  "hwgfx/rect", 
                       "hwgfx/rect_vgrad" )


#can bind individually
shader.bind         ( [( "color0", [1.0,1.0,1.0,0.0] )] )
shader.bind         ( [( "color1", [1.0,1.0,1.0,0.0] )] )

#or as an array

shader.bind         ( [( "color0", [1.0,1.0,1.0,0.0] ),
                      ( "color1", [1.0,1.0,1.0,0.0] )])

# assuming r[] is in the form x,y,w,h, this low-level api call will draw a rectangle 
# primitive, with the "rect" vertex shader and the "rect_vgrad" fragment shader 
hwgfx.rect_draw( r[0],r[1],r[2],r[3] )
```
#note

in this example, we're using the rect shaders which is a dedicated code path in ctt2, which is binding scr_size and rect_coords automatically (by nature of calling hwgfx.rect_draw). If you were creating and rendering your own, you would have to bind those yourself. 


```glsl
#version 330 core

/* shaders/hwgfx/rect.vert.glsl */

in vec2 in_Position;
uniform vec2 scr_size;
uniform vec4 rect_coords;
out vec2 uv;

void main(void) {
    vec2 p_t;

    p_t.x = in_Position.x * rect_coords.z;
    p_t.y = in_Position.y * rect_coords.w;
    p_t.x += rect_coords.x;
    p_t.y += rect_coords.y;
    p_t.x /= scr_size.x;
    p_t.y = scr_size.y - p_t.y;
    p_t.y /= scr_size.y;
    p_t *= 2;
    p_t -= 1;

    uv.x = in_Position.x;
    uv.y = in_Position.y;
    gl_Position = vec4(p_t.x,p_t.y,0.0,1.0);
}
```
```glsl
#version 330 core

/rect_vgrad.frag.glsl

uniform vec4 color0;
uniform vec4 color1;
in vec2 uv;

void main(void) {
    gl_FragColor = ((color0) * uv.y) + ((color1) * (1-uv.y));
}
#version 330 core
```
