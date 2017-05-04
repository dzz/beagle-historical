#include "primitive.h"
#include "shader.h"

void hwgfx_render_test() {

    const gfx_float verts[3][2] = {
        { -1.0, -1.0 },
        { 1.0,-1.0},
        { 1.0,1.0}
    };
    const gfx_float uvs[3][2] = {
        { 0.0, 0.0 },
        { 1.0,0.0},
        { 1.0,1.0}
    };
    gfx_coordinate_uv_primitive primitive;
    gfx_shader shader;
   
    primitive_create_coordinate_uv_primitive( &primitive, (gfx_float*)verts, (gfx_float*)uvs, 3, 2);
    shader_load(&shader, "shaders/test/vert.glsl", "shaders/test/pixel.glsl");
    shader_bind(&shader);

    primitive->mode = GL_TRIANGLES;

    primitive_render( &primitive );
    primitive_destroy_coordinate_uv_primitive( &primitive );

    shader_drop(&shader);
}
