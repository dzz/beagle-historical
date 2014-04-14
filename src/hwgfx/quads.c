#include "shader.h"
#include "primitive.h"
#include "quads.h"

typedef struct{
    gfx_shader solid_shader;
    gfx_coordinate_uv_primitive quad_primitive;
} quad_context;

static quad_context _quad_context;

void initQuads(){
    shader_load( &_quad_context.solid_shader,"shaders/solid_scr.vert.glsl",
            "shaders/solid_scr.frag.glsl");
    primitive_create_scrquad_primitive(&_quad_context.quad_primitive);
}

void dropQuads(){
    shader_drop( &_quad_context.solid_shader);
}

void draw_solidquad(int x, int y, int w, int h, double r, double g, double b) {
    shader_bind( &_quad_context.solid_shader);
    shader_bind_vec3( &_quad_context.solid_shader, "color", r,g,b);
    shader_bind_vec4( &_quad_context.solid_shader, 
            "rect_coords", (float)x,(float)y,(float)w,(float)h);
    shader_bind_vec2( &_quad_context.solid_shader, 
            "scr_size", 1920.0,1080.0);
    primitive_render( &_quad_context.quad_primitive);
}
