#ifndef __HW_BRUSH_CTX__
#define __HW_BRUSH_CTX__

#include <SDL.h>
#include "../document/animation.h"

void initHwBrush();
void dropHwBrush();
void renderLocalBuffer( SDL_Surface* img );
void hw_import_drawing_context( SDL_Surface* img );
void hw_brush_dab(float x, float y, float z,float r,float g,float b,float a, float jit);
void hw_brush_commit_context(SDL_Surface* frdata);
void hw_brush_commit_brush_stroke();
void hw_render_layerstack(frame* fr);

#endif
