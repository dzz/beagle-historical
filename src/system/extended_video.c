#include <SDL.h>
#include <GL/glew.h>

#include "extended_video.h"
#include "../hwgfx/blend_control.h"
#include "../hwgfx/label.h"
#include "../hwgfx/rect.h"
#include "../hwgfx/shader.h"
#include "../hwgfx/context.h"

gfx_texture                     _ui;
gfx_coordinate_uv_primitive     _screen_primitive;
gfx_shader                      _screen_shader;

void gfx_surface_render( SDL_Surface* img) {
    blend_enter                 ( BLENDMODE_OVER        ); 
    shader_bind                 (&_screen_shader        );
    texture_from_SDL_surface    (&_ui,      img         );
    texture_bind                (&_ui,      TEX_UNIT_0  );
    primitive_render            (&_screen_primitive     );
    blend_exit                  (                       );
}

void initGlew(){

    glewExperimental    = GL_TRUE;
    glewInit();

}

void initExtendedVideo() {
    root_gfx_size rgs;   

    initGlew();
    initRects();
    initLabels();

    rgs = gfx_get_root_gfx_size();

    texture_generate                    (&_ui, rgs.w, rgs.h);
    primitive_create_screen_primitive   (&_screen_primitive);
    shader_load                         (&_screen_shader, "shaders/hwgfx/screen.vert.glsl",
                                                          "shaders/hwgfx/texture.frag.glsl");
}

void dropExtendedVideo() {

    primitive_destroy_coordinate_primitive
                                        (&_screen_primitive);
    shader_drop                         (&_screen_shader);
    texture_drop                        (&_ui);
    dropLabels();
    dropRects();
}
