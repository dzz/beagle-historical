#include "texture.y"
#include "quad.h"

gfx_coordinate_uv_primitive _qprim;

void initQuads() {
    const gfx_float verts[4][2] = {
        {-1.0, -1.0},
        { 1.0, -1.0},
        { 1.0,  1.0},
        {-1.0,  1.0} };
    const gfx_float uvs[4][2] = {
        { 0.0,  0.0},
        { 1.0,  0.0},
        { 1.0,  1.0},
        { 0.0,  1.0} };

        primitive_create_coordinate_uv_primitive
            (&_qprim, (gfx_float*)verts, (gfx_float*)uvs, 4, 2);
    }


void quad_render( gfx_quad* quad, gfx_texture* texture) {

    viewport_dims dims = gfx_viewport_get_dims();

    shader_bind(&quad_shader);
    shader_bind_vec2(&quad_shader,  "center",  quad->center_x, quad->center_y);
    shader_bind_vec2(&quad_shader,  "dims",    quad->w, quad->h );
    shader_bind_vec2(&quad_shader,  "scr_size",(float)dims.w, (float)dims.h );
    shader_bind_float(&quad_shader, "rot",     quad->rot );

    texture_bind(texture, TEX_UNIT_0);
}

void dropQuads() {
    primitive_destroy_coordinate_primitive( &_qprim );
}


