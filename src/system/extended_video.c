#include <GL/glew.h>

#include "extended_video.h"
#include "../hwgfx/label.h"
#include "../hwgfx/rect.h"
#include "../hwgfx/context.h"

gfx_texture                 _ui;
gfx_coordinate_primitive    _screen_primitive;

void initGlew(){

    glewExperimental    = GL_TRUE;
    glewInit();

}

void initExtendedVideo() {

    initGlew();
    initRects();
    initLabels();

    root_gfx_size rgs   = gfx_get_root_gfx_size();

    texture_generate                    (_ui,rgs.x, rgs.y);
    primitive_create_screen_primitive   (_screen_primitive);
}

void dropExtendedVideo() {

    primitive_destroy_coordinate_primitive
                                        (_screen_primitive);
    texture_drop                        (_ui);

    dropLabels();
    dropRects();
}


void gfx_surface_render( SDL_Surface* img) {

    blend_enter                 ( BLENDMODE_OVER ); 
    shader_bind                 ( screen_shader);
    texture_from_SDL_surface    (_ui,img);
    texture_bind                (_ui,TEX_UNIT_0);
    primitive_render( &_context.screen_primitive );
    blend_exit();
}
