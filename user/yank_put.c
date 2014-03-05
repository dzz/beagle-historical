#include <SDL.h>

#include "../system/ctt2.h"
#include "../system/dirty.h"

#include "yank_put.h"

static SDL_Surface* buffer;

void initYankPut() {
	SDL_Surface* drawingContext = getDrawingContext();
	buffer = createDrawingSurface( drawingContext->w,
								  drawingContext->h );
}

void dropYankPut() {
	SDL_FreeSurface(buffer);
}

void yankDrawingContext() {
	SDL_BlitSurface( getDrawingContext(), NULL, buffer, NULL );
}

void putDrawingContext() {
	SDL_BlitSurface( buffer, NULL, getDrawingContext(), NULL );
	invalidateDirty(0,0, buffer->w,buffer->h);
}
