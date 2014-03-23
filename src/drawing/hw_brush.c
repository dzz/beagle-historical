#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>

#include "../system/ctt2.h"
#include "hw_brush.h"
#include "../hardware/shader.h"

#define CONTEXT_SIZE 2048

typedef struct {
    GLuint texture_id;
    GLuint vert_array;
    GLuint vert_buffer;

    gfx_shader shader;

} brush_context;

brush_context _context;

const GLfloat verts[4][2] = {
    { -1.0,  -1.0 }, /* Top point */
    {  1.0,  -1.0 }, /* Right point */
    {  1.0, 1.0   }, /* Bottom point */
    { -1.0,  1.0  } }; /* Left point */


unsigned char* generate_debug_texture() {
        unsigned char* texture_data;
        int i;
        int addr=0;

        texture_data = malloc( CONTEXT_SIZE*CONTEXT_SIZE*4,
               sizeof(unsigned char));
        for(i=0; i<(CONTEXT_SIZE*CONTEXT_SIZE);++i) {
            texture_data[addr++]=255;
            texture_data[addr++]=255;
            texture_data[addr++]=255;
            texture_data[addr++]=255;
        }
        return texture_data;
}

void createBrushContext(brush_context *ctxt) {
    unsigned char* texture_data = generate_debug_texture();

    shader_load( &ctxt->shader, "shaders/hw_context.vert.glsl",
                                "shaders/hw_context.frag.glsl" );


    /* bigass texture where we will draw to. when the brush is lifted
     * it will composite down to the drawingContext and get cleared */
    glGenTextures(1,&ctxt->texture_id);
    glBindTexture(GL_TEXTURE_2D,ctxt->texture_id);
#define HWBRUSH_LOD 0 
#define NOBORDER 0 
    glTexImage2D(GL_TEXTURE_2D,HWBRUSH_LOD,GL_RGBA,CONTEXT_SIZE,CONTEXT_SIZE
                ,NOBORDER,
                GL_RGBA, GL_UNSIGNED_BYTE,texture_data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    free(texture_data);

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
    glDeleteTextures(1,&ctxt->texture_id);
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

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ctxt->texture_id );

    glUniform1i(
            glGetUniformLocation( ctxt->shader.shader_id, "ctxt->sampler"),
            0); 

   // glClearColor(1-fr,fr,fr,1.0);
   // glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(ctxt->vert_array);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisable(GL_BLEND);

}

void renderHwBrushContext() {
       _renderBrushContext(&_context);
} 

