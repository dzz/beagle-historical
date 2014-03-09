#ifndef __TOOLBAR__
#define __TOOLBAR__

#include "panels.h"
#include <SDL.h>

#define TOOLBAR_WIDTH (48*4)

#define TOOL_BRUSH 0 
#define TOOL_ERASE 1
#define TOOL_SMUDGE 2
#define TOOL_EYEDROPPER 3

void initToolbar(void);
void destroyToolbar(void);
void renderToolbar(SDL_Surface* target, UI_AREA* area);
void toolbar_mousedown(int x,int y, UI_AREA* area);
unsigned int get_selected_tool(void);

#endif
