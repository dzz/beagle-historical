#ifndef __BRUSH_EDITOR__
#define __BRUSH_EDITOR__

#include "panels.h"
#include <SDL.h>

void renderBrushEditor(SDL_Surface *target, UI_AREA *area);

void brusheditor_mousedown(int x,int y, UI_AREA *area);


#endif
