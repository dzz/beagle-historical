#ifndef __DISPATCH__
#define __DISPATCH__

#include <SDL.h>

#include "../system/ctt2.h"

#include "panels.h"

SYSTEM_SIGNAL dispatch_key(SDL_Keycode sym,int mode);
void dispatch_mousedown( int button,int x,int y);
void dispatch_mouseup(int button,int x, int y);
void dispatch_mousemotion(int x,int y);


#endif
