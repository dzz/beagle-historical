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
    texture_generate(&atlas_texture,1024,1024);
    _atl_cursor = 0;
}

void dropLabels() {
    SDL_FreeSurface(font);
    SDL_FreeSurface(atlas);
    texture_drop(&atlas_texture);
}


void get_cursor_position(int p, int* x,int* y) {
    const int columns = 4;
    int a = p % columns;
    int b = p / columns;
    *x = a * (1024/columns);
    *y = b * (1024/columns);
}

void label_generate(gfx_label* label) {
    label->texture = malloc(sizeof(gfx_texture));
    label->_set = 0;
}

void label_set_text(gfx_label* label, char* text) {
    int i;
    int l = strlen(text);
    SDL_Surface* tex = createDrawingSurface(8*l,8);
    label->w = l*8;
    printf("  %d  ",label->w);
    label->h = 8;
    for( i=0; i<l; ++i) {
        //int val = (int)text[i];
        int val = 1;
        int basex = val % 32;
        int basey = val / 16; 
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
        SDL_Rect r;
        if( label->_set==0) {
            label->_cursor = _atl_cursor;
        }

        get_cursor_position( label->_cursor, &r.x, &r.y );
        r.w = label->w;
        r.h = label->h;

        SDL_BlitSurface(tex,NULL,atlas,&r);
    }
    label->_set = 1; 
    texture_from_SDL_surface(&atlas_texture, tex);
    SDL_FreeSurface(tex);
}

void label_drop(gfx_label* label) {
    free(label->texture);
}
