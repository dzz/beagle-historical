#include "../system/ctt2.h"
#include "../drawing/drawingSurfaces.h"
#include <string.h>
#include <SDL.h>
#include "texture.h"
#include "label.h"

static SDL_Surface* atlas;
static SDL_Surface* font;
gfx_texture atlas_texture;

static int _atl_cursor;

void initLabels() {
    font = IMG_Load("font\\cga8.png");
    atlas = createDrawingSurface(1024,1024);
    printf("ATLAS:%d\n",atlas);
    texture_generate(&atlas_texture,1024,1024);
    _atl_cursor = 1;
}

void dropLabels() {
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

static gfx_label* only_label;

void label_generate(gfx_label* label) {
    only_label = label;
    label->texture = malloc(sizeof(gfx_texture));
    label->_set = 0;
}

void label_set_text(gfx_label* label, const char* text) {
    int i;
    int l;
    SDL_Surface* tex; 

    l = strlen(text);
    tex = createDrawingSurface(8*l,8);
    printf("\n__lbl:%x\n",label);
    printf(" atlas: %d \n",atlas);
    label->w = l*8;
    //printf("  %d  ",label->w);
    label->h = 8;
    for( i=0; i<l; ++i) {
        int val = (int)text[i];
        int basex = val % 32;
        int basey = val / 32; 
        SDL_Rect src;
        SDL_Rect dst;

        src.x=basex*8;
        src.y=basey*8;
        src.w=8;
        src.h=8;

        dst.x=i*8;
        dst.y=0;
        dst.w=8;
        dst.h=8;

        SDL_BlitSurface(font,&src,tex,&dst);
    }
    {
        /* reuse the atlas position if already set */
        SDL_Rect sr;
        SDL_Rect r;
        _pt pt;

        if( label->_set==0) {
            label->_cursor = _atl_cursor;
            _atl_cursor++;
        }

        pt = get_cursor_position(label->_cursor );
        r.x = pt.x;
        r.y = pt.y;
        r.w = label->w;
        r.h = label->h;
        sr.x=0;
        sr.y=0;
        sr.w=label->w;
        sr.h=label->h;

        SDL_BlitSurface(tex,&sr,atlas,&r);
    }
    label->_set = 1; 
    texture_from_SDL_surface(&atlas_texture, atlas);
    SDL_FreeSurface(tex);
}

void label_drop(gfx_label* label) {
    free(label->texture);
}
