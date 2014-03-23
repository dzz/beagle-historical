#ifndef __HW_BRUSH_CTX__
#define __HW_BRUSH_CTX__

#include <SDL.h>


void initHwBrush();
void dropHwBrush();
void renderHwBrushContext();
void renderLocalBuffer( SDL_Surface* img );
#endif
