#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>

#include "../hwgfx/shader.h"
#include "../hwgfx/texture.h"
#include "../hwgfx/primitive.h"

#include "hw_brush.h"

#define CONTEXT_SIZE 2048

typedef struct {
    GLuint vert_array;
    GLuint vert_buffer;

    gfx_shader shader;
    gfx_texture texture;
    gfx_coordinate_primitive primitive;

} brush_context;

brush_context _context;

const gfx_float verts[4][2] = {
    {  0.0, 0.0 }, 
    {  1.0, 0.0 }, 
    {  1.0, 1.0 }, 
    {  0.0, 1.0 } }; 

void createBrushContext(brush_context *ctxt) {

    shader_load( &ctxt->shader, "shaders/hw_context.vert.glsl",
                                "shaders/hw_context.frag.glsl" );

    /* bigass texture where we will draw to. when the brush is lifted
     * it will composite down to the drawingContext and get cleared */

    texture_generate( &ctxt->texture, CONTEXT_SIZE,
                                      CONTEXT_SIZE);

    primitive_create_coordinate_primitive(&ctxt->primitive,
            verts, 4);


}

void destroyBrushContext(brush_context *ctxt) {
    shader_drop(&ctxt->shader);
    texture_drop(&ctxt->texture);
    primitive_destroy_coordinate_primitive(&ctxt->primitive);
}

void initHwBrush(){
    createBrushContext(&_context);
}

void dropHwBrush(){ 
    destroyBrushContext(&_context);
}


void _renderBrushContext(brush_context* ctxt) {
    shader_bind( &ctxt->shader);
    texture_bind( &ctxt->texture, TEX_UNIT_0 );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    primitive_render_coordiate_primitive( &ctxt->primitive );
    glDisable(GL_BLEND);

}

void renderHwBrushContext() {
       _renderBrushContext(&_context);
} 

