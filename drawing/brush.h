#ifndef __BRUSH__
#define __BRUSH__

#include "../user/stylus.h"
#include <SDL.H>

void initBrush(SDL_Surface* context);
void brushPaint(stylusState a, stylusState b);
void brushReset();

#endif
