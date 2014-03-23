#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>

#include "../hwgfx/shader.h"
#include "../hwgfx/texture.h"
#include "../hwgfx/primitive.h"

#include "hw_brush_context.h"

#define CONTEXT_SIZE 2048

typedef struct {
    gfx_shader shader;
    gfx_texture texture;
    gfx_texture ui;
    gfx_coordinate_primitive primitive;

} brush_context;

brush_context _context;

void createBrushContext(brush_context *ctxt) {

    shader_load( &ctxt->shader, "shaders/hw_context.vert.glsl",
                                "shaders/hw_context.frag.glsl" );

    /* bigass texture where we will draw to. when the brush is lifted
     * it will composite down to the drawingContext and get cleared */

    texture_generate( &ctxt->texture, CONTEXT_SIZE,
                                      CONTEXT_SIZE);

    texture_generate( &ctxt->ui, 1920,1080 );

    primitive_create_screen_primitive(&ctxt->primitive);


}

void destroyBrushContext(brush_context *ctxt) {
    shader_drop(&ctxt->shader);
    texture_drop(&ctxt->texture);
    primitive_destroy_coordinate_primitive(&ctxt->primitive);
    texture_drop(&ctxt->ui);
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
    primitive_render( &ctxt->primitive );
}

void renderHwBrushContext() {
       _renderBrushContext(&_context);
} 

void renderLocalBuffer( SDL_Surface* img) {
    shader_bind( &_context.shader);
    texture_from_SDL_surface(&_context.ui,img);
    texture_bind(&_context.ui,TEX_UNIT_0);
    primitive_render( &_context.primitive );
}

