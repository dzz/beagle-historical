#ifndef __DRAWING_CONTEXT__
#define __DRAWING_CONTEXT__

#include <SDL.h>

void initDrawingContext();
void dropDrawingContext();

SDL_Surface *getDrawingContext(void);

#endif

