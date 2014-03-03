#ifndef __DISPATCH__
#define __DISPATCH__

#include <SDL.H>

#include "panels.h"

void dispatch_key(SDL_Keycode sym,int mode);
void dispatch_mousedown( int button,int x,int y);
void dispatch_mouseup(int button,int x, int y);
void dispatch_mousemotion(int x,int y);

int pointInArea(int x, int y, UI_AREA area);

#endif
