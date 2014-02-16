#ifndef __DRAWING_SURFACES__
#define __DRAWING_SURFACES__
#include <SDL.h>

typedef SDL_Surface* DRAWING_CONTEXT;
typedef SDL_Surface* DRAWING_SURFACE;

SDL_Surface* createDrawingSurface(int w, int h);
void dropDrawingSurfaces();
SDL_Surface * getConvertedForBlit(SDL_Surface *drawingContext, SDL_Surface *screenSurface);

#endif
