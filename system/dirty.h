#ifndef __DIRTY__
#define __DIRTY__
#include <SDL.H>

void invalidateDirty(int x, int y, int x2, int y2);
void resetDirty();
SDL_Rect getDirtyRect();

#endif
