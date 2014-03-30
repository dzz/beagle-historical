#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>

#include "../system/surfaceCache.h"

#include "../document/animation.h"
#include "../document/layers.h"

#include "../hwgfx/blend_control.h"
#include "../hwgfx/shader.h"
#include "../hwgfx/texture.h"
#include "../hwgfx/framebuffer.h"
#include "../hwgfx/primitive.h"
#include "../hwgfx/misc.h"

#include "hw_brush_context.h"

typedef struct {
    gfx_shader shader;
    gfx_shader dab_shader;
    gfx_texture context_texture;
    gfx_texture ui;
    gfx_coordinate_primitive screen_primitive;
    gfx_coordinate_primitive dab_primitive;
    gfx_framebuffer framebuffer;
} brush_context;

brush_context _context;

void createBrushContext(brush_context *ctxt) {
    shader_load( &ctxt->shader, "shaders/hw_context.vert.glsl",
                                "shaders/hw_context.frag.glsl" );

    shader_load( &ctxt->dab_shader, "shaders/dab.vert.glsl",
                               "shaders/dab.frag.glsl" );

    texture_generate( &ctxt->context_texture, 1920, 1080); 
    texture_generate( &ctxt->ui, 1920, 1080);

    primitive_create_screen_primitive(&ctxt->screen_primitive);
    primitive_create_dab_primitive(&ctxt->dab_primitive);

    framebuffer_create_framebuffer(&ctxt->framebuffer);
    framebuffer_bind_texture(&_context.framebuffer, &_context.context_texture);
}

void destroyBrushContext(brush_context *ctxt) {
    shader_drop(&ctxt->dab_shader);
    shader_drop(&ctxt->shader);
    
    texture_drop(&ctxt->context_texture);
    texture_drop(&ctxt->ui);
   
    framebuffer_drop(&ctxt->framebuffer); 
    primitive_destroy_coordinate_primitive(&ctxt->screen_primitive);
    primitive_destroy_coordinate_primitive(&ctxt->dab_primitive);
}

void hw_brush_dab(float x,float y,float z, float r,float g, float b,float a, float jit) {
    framebuffer_render_start(&_context.framebuffer);
    {
        blend_enter( BLENDMODE_OVER ); {
            shader_bind( &_context.dab_shader );
            shader_bind_vec3( &_context.dab_shader, "dab_location",x,y,z );
            shader_bind_vec4( &_context.dab_shader, "base_color",r,g,b,a );
            shader_bind_vec2( &_context.dab_shader, "scr_size",1920.0, 1080.0);
            shader_bind_float(&_context.dab_shader, "jitter",jit); 
            primitive_render( &_context.dab_primitive);
        }
        blend_exit();
    }
    framebuffer_render_end(&_context.framebuffer);
}

void initHwBrush(){
    createBrushContext(&_context);
}

void dropHwBrush(){ 
    destroyBrushContext(&_context);
}

void _renderBrushContext(brush_context* ctxt) {
    shader_bind( &ctxt->shader);
    texture_bind( &ctxt->context_texture, TEX_UNIT_0 );
    primitive_render( &ctxt->screen_primitive );
}

void renderHwBrushContext() {
       _renderBrushContext(&_context);
} 

void renderLocalBuffer( SDL_Surface* img) {
    blend_enter( BLENDMODE_OVER ); {
        shader_bind( &_context.shader);
        texture_from_SDL_surface(&_context.ui,img);
        texture_bind(&_context.ui,TEX_UNIT_0);
        primitive_render( &_context.screen_primitive );
    } 
    blend_exit();
}

void importBrushContext( SDL_Surface* img) {
        texture_from_SDL_surface(&_context.context_texture,img);
}

void hw_brush_commit_context() {
    SDL_Surface* frdata = 
        getCompositeLayerFromFrame(getActiveFrame(),
                                   getActiveLayer(), 
                                   COMP_RESOLVE_VIRTUAL) ->data;

    texture_download( &_context.context_texture, frdata);
    framebuffer_render_start(&_context.framebuffer);
    gfx_clear();
    framebuffer_render_end(&_context.framebuffer);
}
