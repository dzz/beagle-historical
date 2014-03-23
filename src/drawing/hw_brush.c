#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>

#include "../hardware/shader.h"
#include "../hardware/texture.h"
#include "hw_brush.h"

#define CONTEXT_SIZE 2048

typedef struct {
    GLuint vert_array;
    GLuint vert_buffer;

    gfx_shader shader;
    gfx_texture texture;

} brush_context;

brush_context _context;

const GLfloat verts[4][2] = {
    {  0.0, 0.0 }, /* Top point */
    {  1.0, 0.0 }, /* Right point */
    {  1.0, 1.0 }, /* Bottom point */
    {  0.0, 1.0 } }; /* Left point */



void createBrushContext(brush_context *ctxt) {

    shader_load( &ctxt->shader, "shaders/hw_context.vert.glsl",
                                "shaders/hw_context.frag.glsl" );

    /* bigass texture where we will draw to. when the brush is lifted
     * it will composite down to the drawingContext and get cleared */

    texture_generate( &ctxt->texture, CONTEXT_SIZE,
                                      CONTEXT_SIZE);

    /* gemoetry */
    glGenVertexArrays(1, &ctxt->vert_array);
    glBindVertexArray(ctxt->vert_array);
    glGenBuffers(1, &ctxt->vert_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, ctxt->vert_buffer);
#define COORDINATE_ATTRIBUTE_INDEX  0
#define COLOR_ATTRIBUTE_INDEX 1

#define FLOATS_PER_VERT 2

#define NOT_NORMALIZED 0
#define NO_STRIDE 0
#define NO_POINTER_OFFSET 0

    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), verts, GL_STATIC_DRAW);

    glVertexAttribPointer(COORDINATE_ATTRIBUTE_INDEX, 
            FLOATS_PER_VERT, GL_FLOAT, NOT_NORMALIZED, 
            NO_STRIDE, 
            NO_POINTER_OFFSET); 

    //would bind uv etc. here
    glEnableVertexAttribArray(COORDINATE_ATTRIBUTE_INDEX);

}

void destroyBrushContext(brush_context *ctxt) {

    shader_drop(&ctxt->shader);
    texture_drop(&ctxt->texture);

    glDeleteVertexArrays(1,&ctxt->vert_array);
    glDeleteBuffers(1,&ctxt->vert_buffer);
}

void initHwBrush(){
    createBrushContext(&_context);
}

void dropHwBrush(){ 
    destroyBrushContext(&_context);
}

int oscillator = 0;

void _renderBrushContext(brush_context* ctxt) {
    int i;
    float fr = (float)(oscillator%2)/10.0f;

    oscillator ++;

    shader_bind( &ctxt->shader);
    texture_bind( &ctxt->texture, TEX_UNIT_0 );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glClearColor(1-fr,fr,fr,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(ctxt->vert_array);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisable(GL_BLEND);

}

void renderHwBrushContext() {
       _renderBrushContext(&_context);
} 

