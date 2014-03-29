#ifndef __HW_BRUSH_CTX__
#define __HW_BRUSH_CTX__

#include <SDL.h>

void initHwBrush();
void dropHwBrush();
void renderHwBrushContext();
void renderLocalBuffer( SDL_Surface* img );
void importBrushContext( SDL_Surface* img );
void hw_brush_dab(float x, float y, float z,float r,float g,float b,float a);

#endif
