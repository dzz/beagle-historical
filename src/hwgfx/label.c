#include "../drawing/drawingSurfaces.h"
#include <string.h>
#include <SDL.h>
#include "texture.h"
#include "label.h"

typedef struct {
    gfx_texture texture;
    int w;
    int h; 
} gfx_label;

gfx_label label_generate(char* text) {
    gfx_label label;
    SDL_Surface* font = IMG_Load("font\\cga8.png");
    int i;
    int l = strlen(text);
    SDL_Surface* tex = createDrawingSurface(8*l,8);

    label.w = l*8;
    label.h = 8;

    for( i=0; i<l; ++i) {
       int val = (int)text[i];
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
    texture_from_SDL_surface(&label.texture, tex);
    SDL_FreeSurface(font);
    SDL_FreeSurface(tex);
    return label;
}

void label_drop(gfx_label label) {
    texture_drop(&label.texture);
}
