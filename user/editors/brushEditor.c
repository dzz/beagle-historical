#include <SDL.h>
#include <math.h>

#include "brushEditor.h"

#include "../drawing/drawingSurfaces.h"
#include "../drawing/brush.h"


static SDL_Surface* bg;
static SDL_Surface* slider;
static SDL_Surface* handle;
static SDL_Surface* preview_bg;

double sVals[32];

SDL_Surface* slider_images[64];

double get_brusheditor_value(int idx) {
	return sVals[idx];
}

void redraw_stroke_sample() {
	//285 to 385, and same height as sliders
	SDL_Surface *brushSample = createDrawingSurface(128,255);
	SDL_Rect rect;
	

	brush_reset_random();

	SDL_BlitSurface(preview_bg,NULL,brushSample,NULL);

	rect.x = 64-4;
	rect.y = 4;
	rect.w = 64;
	rect.h = 64;

	SDL_BlitSurface(brush_get_active_dab_bmp(),NULL,brushSample,&rect);
	{
		int v;
		for( v = 0; v< 25; ++v ) {
				unsigned int t0 = (1000/25)*v;
				float p = (float)v/25;
				int x1 = (int)((cosf((p*3.14*4)))*10) + 57;
				int x2 = (int)((cosf(((p+(1.0/25))*3.14*4)))*10) + 57;

				node_mapper_apply_input(p);
				brush_tesselate_stroke(
								x1,(v*8)+15,x2,(v*8)+8+15,p,p,t0,t0,brushSample);

		}
	}

	rect.x = 128;
	rect.y = 0;
	rect.w = 128;
	rect.h = 255;

	SDL_BlitSurface(brushSample,NULL,bg,&rect);
	SDL_FreeSurface(brushSample);

}

void brusheditor_redraw_stroke() {
	redraw_stroke_sample();
}

void initBrushEditor() {
	bg = SDL_LoadBMP("ui_gen/be_bg.bmp");
	slider = SDL_LoadBMP("ui_gen/be_slider.bmp");
	handle = SDL_LoadBMP("be_handle.bmp");
	preview_bg = SDL_LoadBMP("be_preview.bmp");

	sVals[0] = 0.25; //size
	sVals[1] = 0.0; //dab

	slider_images[0] = SDL_LoadBMP("ui_gen/slider_0.bmp");
	slider_images[1] = SDL_LoadBMP("ui_gen/slider_1.bmp");
	slider_images[2] = SDL_LoadBMP("ui_gen/slider_2.bmp");
	slider_images[3] = SDL_LoadBMP("ui_gen/slider_3.bmp");
	slider_images[4] = SDL_LoadBMP("ui_gen/slider_4.bmp");
	slider_images[5] = SDL_LoadBMP("ui_gen/slider_5.bmp");

	brush_setValuesFromUI();
	redraw_stroke_sample();
}

void destroyBrushEditor() {
	int i;
	for(i=0;i<6;++i) {
		SDL_FreeSurface(slider_images[i]);
	}

	SDL_FreeSurface(bg);
	SDL_FreeSurface(slider);
	SDL_FreeSurface(handle);
	SDL_FreeSurface(preview_bg);
}

void renderBrushEditor(SDL_Surface *target, UI_AREA *area) {
		int i;
		SDL_Rect spos;

		SDL_BlitSurface(bg,NULL,target,(SDL_Rect*)area);

		for(i=0; i<2; ++i) {
			spos.x = (area->x + (i*35))+10; //5px margin
			spos.y = area->y + 6; //12px margin
			spos.w = 30;
			spos.h = 244;
			SDL_BlitSurface(slider_images[i], NULL,target,&spos);

			spos.y = (int)(((1-sVals[i])*(spos.h-12))+spos.y-8)+6;
			spos.h = 16;
			SDL_BlitSurface(handle, NULL,target,&spos);
		}

}

static int lockedSlider = -1;
static int motion_redraw_step = 0;
static const int motion_redraw_cycles = 4;


void map_to_slider_value(int *x, int *y,double *p) {
	(*x) = (*x-10) / 35;
	(*y) = (*y-6);
	(*p) = (double)(*y) / 244.0;

	if((*p)>1) (*p)=1;
	if((*p)<0) (*p)=0;
		(*p) = 1-(*p);
}

void brusheditor_mouseleave() {
	lockedSlider = -1;
}

void brusheditor_mousemotion(int x,int y, UI_AREA *area) {
	motion_redraw_step++;
	motion_redraw_step %= motion_redraw_cycles;
	if(lockedSlider!=-1) {
		double value;
		map_to_slider_value(&x,&y,&value);
		sVals[lockedSlider] = value;
		if(motion_redraw_step == 0 ) {
			brush_setValuesFromUI();
			redraw_stroke_sample();
		}
	}
}
void brusheditor_mousedown(int x,int y, UI_AREA *area) {
		double value;
		map_to_slider_value(&x,&y,&value);
		if(x<0)
				return;
		{
				lockedSlider = x;
				sVals[x] = value;
				brush_setValuesFromUI();
				redraw_stroke_sample();
		}
}

void brusheditor_mouseup(int x,int y, UI_AREA *area) {
		lockedSlider = -1;
}
