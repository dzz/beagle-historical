#include "toolbar.h"

#include <SDL.h>

#define MAX_BUTTONS 4
#define BUTTON_SIZE 48

SDL_Surface* toolbarBmps[MAX_BUTTONS*2];
static unsigned int selected_tool = 0;
static unsigned int loaded_tools = 0;

unsigned int get_selected_tool(void){
	return selected_tool;
}

void initToolbar(void) {
		int idx;
		for(idx=0; idx< MAX_BUTTONS; ++idx) {
			char fname[1024];
			sprintf(fname,"ui_gen/toolbar_%d_up.bmp",idx);
			toolbarBmps[idx] = SDL_LoadBMP(fname);
			if(toolbarBmps[idx]==0){
					break;
			}
			sprintf(fname,"ui_gen/toolbar_%d_down.bmp",idx);
			toolbarBmps[idx+MAX_BUTTONS] = SDL_LoadBMP(fname);
			loaded_tools++;
		}
}

void toolbar_mousedown(int x,int y, UI_AREA* area) {
	int idx = x / BUTTON_SIZE;
	if( idx<=loaded_tools ) {
		selected_tool = idx;
	}	
}

void renderToolbar(SDL_Surface* target, UI_AREA* area) {
	int i;
	for(i=0; i< loaded_tools; ++i) {
		SDL_Rect r; 
		r.x = area->x+i*BUTTON_SIZE;
		r.y = area->y;
		r.w = BUTTON_SIZE;
		r.h = BUTTON_SIZE;

		if(i==selected_tool) {
			SDL_BlitSurface(toolbarBmps[i+MAX_BUTTONS],NULL,target,&r);
		} else {
			SDL_BlitSurface(toolbarBmps[i],NULL,target,&r);
		}
	}
}

void destroyToolbar(void) {
	int idx;
	for(idx=0; idx< MAX_BUTTONS; ++idx) {
		if(toolbarBmps[idx]==0) {
				break;
		}
		SDL_FreeSurface(toolbarBmps[idx]);
		SDL_FreeSurface(toolbarBmps[idx + MAX_BUTTONS]);
	}
}
