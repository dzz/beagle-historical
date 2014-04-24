#include "../system/ctt2.h"
#include "../drawing/drawingSurfaces.h"
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include "context.h"
#include "texture.h"
#include "shader.h"
#include "primitive.h"
#include "label.h"

static SDL_Surface* atlas;
static SDL_Surface* font;
gfx_shader  label_shader;
gfx_texture atlas_texture;

static int _atl_cursor;


void initLabels() {
    font        = IMG_Load("font\\cga8.png");
    atlas       = createDrawingSurface(1024,1024);
    _atl_cursor = 0;

    texture_generate(&atlas_texture,1024,1024);
    shader_load(&label_shader, "shaders/hwgfx/label.vert.glsl",
                               "shaders/hwgfx/label.frag.glsl");
}

void dropLabels() {

    shader_drop(&label_shader);
    SDL_FreeSurface(font);
    SDL_FreeSurface(atlas);
    texture_drop(&atlas_texture);
}


typedef struct{
    int x;
    int y;
} _pt;

_pt get_cursor_position(int p) {
    _pt r;
    int a = p % 4;
    int b = p / 4;

    r.x = a * (1024/4);
    r.y = b*8;
    return r;
}


void label_render(gfx_label* label, float x, float y,float r,float g, float b) {

    viewport_dims dims = gfx_viewport_get_dims();

    shader_bind         (&label_shader                              );
    shader_bind_vec2    (&label_shader, "label_pos",   x,y          );
    shader_bind_vec2    (&label_shader, "scr_size",    (float)dims.w,(float)dims.h);
    shader_bind_vec3    (&label_shader, "label_col",   r,g,b        );
    texture_bind        (&atlas_texture, TEX_UNIT_0                 );

    primitive_render    ((gfx_coordinate_primitive*)label->primitive);
}

void label_generate(gfx_label* label) {
    float uvw,uvh;
    float uo,vo;
    _pt pt;
    label->texture      = malloc(sizeof(gfx_texture));
    label->primitive    = malloc(sizeof(gfx_coordinate_uv_primitive));
    label->_cursor      = _atl_cursor; 
    _atl_cursor++; 

    pt = get_cursor_position(label->_cursor);

    uvw = 256.0/1024.0;
    uvh = 8.0/1024.0;
    uo = (float)pt.x / 1024.0f;
    vo = (float)pt.y / 1024.0f;

    {
        const gfx_float verts[4][2] = {
            {0.0, 0.0},
            {256.0,0.0},
            {256.0,8.0},
            {0.0,8.0} };
        const gfx_float uvs[4][2] = {
            {uo,     vo    },
            {uo+uvw, vo    },
            {uo+uvw, vo+uvh},
            {uo,     vo+uvh} };
        primitive_create_coordinate_uv_primitive
            (label->primitive,
             (gfx_float*)verts, (gfx_float*)uvs, 4, 2);
    }
          
}

void label_set_text(gfx_label* label, const char* text) {
    int i;
    int l;
    SDL_Surface* tex; 

    l = strlen(text);
    tex = createDrawingSurface(8*l,8);
    label->w = l*8;
    label->h = 8;
    for( i=0; i<l; ++i) {
        int val = (int)text[i];
        int basex = val % 32; int basey = val / 32; 

        SDL_Rect src; SDL_Rect dst;

        src.x=basex*8;  src.y=basey*8;
        src.w=8;        src.h=8;

        dst.x=i*8;      dst.y=0;
        dst.w=8;        dst.h=8;

        SDL_BlitSurface(font,&src,tex,&dst);
    }
    {
        SDL_Rect sr; SDL_Rect r;
        _pt pt;

        pt = get_cursor_position(label->_cursor );
        /*src*/
        sr.x=0;     sr.y=0;
        sr.w=label->w; sr.h=label->h;
        /*dst*/
        r.x = pt.x;     r.y = pt.y;
        r.w = label->w; r.h = label->h;

        SDL_BlitSurface(tex,&sr,atlas,&r);
    }
    texture_from_SDL_surface(&atlas_texture, atlas);
    SDL_FreeSurface(tex);
}

void label_drop(gfx_label* label) {
    texture_drop(label->texture);
    free(label->texture);
    primitive_destroy_coordinate_primitive((gfx_coordinate_primitive*)label->primitive);
    free(label->primitive);
}
