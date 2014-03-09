#include <SDL.h>
#include "panels.h"
#include "nodeMapEditor.h"

void initNodeMapEditor(){}
void destroyNodeMapEditor(){}

void renderNodeMapEditor(SDL_Surface* target, UI_AREA* area){

		unsigned int bg_color = SDL_MapRGB(target->format,
						16,16,16);
		SDL_FillRect(target, (SDL_Rect*) area, bg_color);
}

void nodemapeditor_mousedown(int x,int y, UI_AREA* area){}
