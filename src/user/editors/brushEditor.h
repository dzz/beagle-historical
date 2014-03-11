#ifndef __BRUSH_EDITOR__
#define __BRUSH_EDITOR__

#include <SDL.h>

#include "../panels.h"

void initBrushEditor();
void destroyBrushEditor();

void renderBrushEditor(SDL_Surface* target, UI_AREA* area);

void brusheditor_mousedown(int x,int y, UI_AREA* area);
void brusheditor_mouseup(int x,int y, UI_AREA* area);
void brusheditor_mousemotion(int x, int y,UI_AREA* area);
void brusheditor_mouseleave();

void brusheditor_redraw_stroke();

double get_brusheditor_value(int idx);

#endif
