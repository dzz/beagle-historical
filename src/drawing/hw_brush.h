#ifndef __HW_BRUSH__
#define __HW_BRUSH__

#include <SDL.h>


void initHwBrush();
void dropHwBrush();
void renderHwBrushContext();
void renderLocalBuffer( SDL_Surface* img );
#endif
