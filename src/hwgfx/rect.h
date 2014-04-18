#ifndef __GFX_QUADS__
#define __GFX_QUADS__

#include "shader.h"

void initRects();
void draw_solid_rect
    (int x, int y, int w, int h, double r, double g, double b, double a);
void draw_rect
    (int x,int y, int w, int h, gfx_shader* bound_shader);

void dropRects();

#endif
