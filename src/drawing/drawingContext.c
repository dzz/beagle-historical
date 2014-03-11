#include <SDL.h>
#include "drawingContext.h"

static SDL_Surface *drawingContext;

void dropDrawingContext() {
	SDL_FreeSurface(drawingContext);
}

void initDrawingContext() {
	drawingContext = createDrawingSurface(1920,1080);
}

SDL_Surface *getDrawingContext() {
	return drawingContext;
}
