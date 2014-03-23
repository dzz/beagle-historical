#ifndef __GFX_PRIMITIVE__
#define __GFX_PRIMITIVE__

#include <GL/glew.h>

typedef struct {
    GLuint vert_array;
    GLuint vert_buffer;
    int _num_verts;
} gfx_coordinate_primitive;

typedef struct{
  GLuint vert_array;
  GLuint vert_buffer;
  GLuint uv_buffer;
  int _num_verts;
} gfx_coordinate_uv_primitive;

typedef GLfloat gfx_float;

void primitive_create_coordinate_primitive(gfx_coordinate_primitive* 
        primitive, int verts);
void primitive_destroy_coordinate_primitive(gfx_coordinate_primitive*
        primitive);
void primitive_render_coordiate_primitive(gfx_coordinate_primitive*
        primitive);


void primitive_create_dab_primitive(gfx_coordinate_uv_primitive*
        uv_primitive);
void primitive_destroy_coordinate_uv_primitive(gfx_coordinate_uv_primitive*
        uv_primitive);
void primitive_render_coordinate_uv_primitive(gfx_coordinate_uv_primitive*
        uv_primitive);

#endif
