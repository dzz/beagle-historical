#include "brushEditor.h"
#include <SDL.h>

static SDL_Surface* bg;

void initBrushEditor() {
	bg = SDL_LoadBMP("be_bg.bmp");
}

void destroyBrushEditor() {

}

void renderBrushEditor(SDL_Surface *target, UI_AREA *area) {
		SDL_BlitSurface(bg,NULL,target,area);
}

void brusheditor_mousedown(int x,int y, UI_AREA *area) {
}
