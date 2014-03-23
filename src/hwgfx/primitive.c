#include <GL/glew.h>

#include "primitive.h"

#define COORDINATE_ATTRIBUTE_INDEX  0
#define UV_ATTRIBUTE_INDEX 1
#define FLOATS_PER_VERT 2

#define NOT_NORMALIZED 0
#define NO_STRIDE 0
#define NO_POINTER_OFFSET 0


void primitive_create_coordinate_primitive(gfx_coordinate_primitive* 
        primitive, gfx_float* coordinates, int verts ){

    glGenVertexArrays(1, &primitive->vert_array);
    glBindVertexArray(primitive->vert_array);
    glGenBuffers(1, &primitive->vert_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, primitive->vert_buffer);
    glBufferData(GL_ARRAY_BUFFER, (verts*FLOATS_PER_VERT) * sizeof(GLfloat), 
            coordinates, 
            GL_STATIC_DRAW);
    primitive->_num_verts = verts;
        //(sizeof(coordinates) / sizeof(GLfloat)) / FLOATS_PER_VERT;

    glVertexAttribPointer(COORDINATE_ATTRIBUTE_INDEX, 
            FLOATS_PER_VERT, 
            GL_FLOAT, /*datatype*/
            NOT_NORMALIZED, 
            NO_STRIDE, 
            NO_POINTER_OFFSET ); 

    glEnableVertexAttribArray(COORDINATE_ATTRIBUTE_INDEX);
}

void primitive_destroy_coordinate_primitive(gfx_coordinate_primitive*
        primitive){

    glDeleteVertexArrays(1,&primitive->vert_array);
    glDeleteBuffers(1,&primitive->vert_buffer);

}

void primitive_render_coordiate_primitive(gfx_coordinate_primitive*
        primitive) {
    glBindVertexArray(primitive->vert_array);
    glDrawArrays(GL_TRIANGLE_FAN, 0, primitive->_num_verts);
}
