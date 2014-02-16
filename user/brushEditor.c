#include "brushEditor.h"
#include <SDL.h>

static SDL_Surface* bg;
static SDL_Surface* slider;
static SDL_Surface* handle;

double sVals[32];
SDL_Surface* icons[64];

double get_brusheditor_value(int idx) {
	return sVals[idx];
}

void initBrushEditor() {
	bg = SDL_LoadBMP("be_bg.bmp");
	slider = SDL_LoadBMP("be_slider.bmp");
	handle = SDL_LoadBMP("be_handle.bmp");
	sVals[0] = 0.5;
	sVals[1] = 0.5;
	sVals[2] = 0.5;
	sVals[4] = 1;
}

void destroyBrushEditor() {
	SDL_FreeSurface(bg);
	SDL_FreeSurface(slider);
	SDL_FreeSurface(handle);
}

void renderBrushEditor(SDL_Surface *target, UI_AREA *area) {
		int i;
		SDL_Rect spos;

		SDL_BlitSurface(bg,NULL,target,area);

		//our slider images are 30x244 and we have a
		// 400x256 area to fill with sliders

		for(i=0; i<3; ++i) {
			spos.x = (area->x + (i*50))+5; //5px margin
			spos.y = area->y + 6; //12px margin
			spos.w = 30;
			spos.h = 244;
			SDL_BlitSurface(slider, NULL,target,&spos);

			spos.y = (int)((1-sVals[i])*spos.h)+spos.y;
			spos.h = 16;
			SDL_BlitSurface(handle, NULL,target,&spos);
		}
}

void brusheditor_mousedown(int x,int y, UI_AREA *area) {
		int idx = x / 50;
		int vert = (y-6);

		sVals[idx] = (double)vert / 244.0;
		if(sVals[idx]>1) sVals[idx]=1;
		sVals[idx] = 1-sVals[idx];
}
