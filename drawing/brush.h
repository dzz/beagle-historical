#ifndef __BRUSH__
#define __BRUSH__

#include "../user/stylus.h"
#include <SDL.H>

void initBrush(SDL_Surface* context);
void brushPaint(stylusState a, stylusState b);
void brushReset();
void brush_drawStrokeSegment(int x0,int y0,int x1,int y1,float p0,float p1, SDL_Surface *ctxt);
void brush_setValuesFromUI();
#endif
