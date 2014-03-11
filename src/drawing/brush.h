#ifndef __BRUSH__
#define __BRUSH__

#include "../user/stylus.h"
#include <SDL.h>

void initBrush(SDL_Surface* context);
void destroyBrush();

void brush_begin_stroke(stylusState a);
void brush_render_stylus_stroke(stylusState a, stylusState b);
void brush_end_stroke();
void brush_tesselate_stroke(int x0,int y0,int x1,int y1,float p0,float p1, unsigned int t0, unsigned int t1, SDL_Surface *ctxt);

void brush_setValuesFromUI();
void brush_toggleMixMode();
void brush_reset_random();

SDL_Surface *brush_get_active_dab_bmp();

#endif
