#include "brushEditor.h"
#include "../drawing/drawingSurfaces.h"
#include "../drawing/brush.h"
#include "h_slider.h"

#include <SDL.h>

static SDL_Surface* bg;
static SDL_Surface* slider;
static SDL_Surface* handle;
static SDL_Surface* preview_bg;

double sVals[32];
SDL_Surface* slider_images[64];
SDL_Surface* button_images[64];

double get_brusheditor_value(int idx) {
	return sVals[idx];
}

void brusheditor_redraw_stroke() {
	redraw_stroke_sample();
}

void redraw_stroke_sample() {
	//285 to 385, and same height as sliders
	SDL_Surface *brushSample = createDrawingSurface(100,244);
	SDL_BlitSurface(preview_bg,NULL,brushSample,NULL);
	brush_setValuesFromUI();
	{
		int v;
		for( v = 0; v< 200; ++v ) {
				float p = (float)v/200;
				int x = (int)((cosf((p*3.14*4)))*10) + 57;
				brush_drawStrokeSegment(
								x,v,x,v+1,p,p,brushSample);
		}
	}

	SDL_Rect rect;
	rect.x = 295;
	rect.y = 6;
	rect.w = 100;
	rect.h = 244;

	SDL_BlitSurface(brushSample,NULL,bg,&rect);
	SDL_FreeSurface(brushSample);
}

int button_origin_x;
int button_origin_y;

SDL_Rect get_buttons_rect() {
	SDL_Rect r;
	r.w = 45;
	r.h = 20;

	r.x = 230;
	r.y = 7;
	return r;
}
void draw_buttons_to_bg() {
	SDL_Rect r= get_buttons_rect();	
	SDL_BlitSurface(button_images[0],NULL,bg,&r);
	r.y+=r.h;
	SDL_BlitSurface(button_images[2],NULL,bg,&r);
	r.y+=r.h;
	SDL_BlitSurface(button_images[4],NULL,bg,&r);
	r.y+=r.h;
	SDL_BlitSurface(button_images[6],NULL,bg,&r);
	r.y+=r.h;
	SDL_BlitSurface(button_images[8],NULL,bg,&r);
	r.y+=r.h;
}

int get_brusheditor_toggle(int idx) {
	return 1;
}

int get_brusheditor_radio() {
	return 0;
}

void initBrushEditor() {
	bg = SDL_LoadBMP("be_bg.bmp");
	slider = SDL_LoadBMP("be_slider.bmp");
	handle = SDL_LoadBMP("be_handle.bmp");
	preview_bg = SDL_LoadBMP("be_preview.bmp");

	sVals[0] = 0.5;
	sVals[1] = 0.5;
	sVals[2] = 0.5;
	sVals[3] = 1;
	sVals[4] = 0.0;
	sVals[5] = 0.0;

	slider_images[0] = SDL_LoadBMP("ui_gen/slider_0.bmp");
	slider_images[1] = SDL_LoadBMP("ui_gen/slider_1.bmp");
	slider_images[2] = SDL_LoadBMP("ui_gen/slider_2.bmp");
	slider_images[3] = SDL_LoadBMP("ui_gen/slider_3.bmp");
	slider_images[4] = SDL_LoadBMP("ui_gen/slider_4.bmp");
	slider_images[5] = SDL_LoadBMP("ui_gen/slider_5.bmp");

	button_images[0] = SDL_LoadBMP("ui_gen/button_0_up.bmp");
	button_images[1] = SDL_LoadBMP("ui_gen/button_0_down.bmp");
	button_images[2] = SDL_LoadBMP("ui_gen/button_1_up.bmp");
	button_images[3] = SDL_LoadBMP("ui_gen/button_1_down.bmp");
	button_images[4] = SDL_LoadBMP("ui_gen/button_2_up.bmp");
	button_images[5] = SDL_LoadBMP("ui_gen/button_2_down.bmp");
	button_images[6] = SDL_LoadBMP("ui_gen/button_3_up.bmp");
	button_images[7] = SDL_LoadBMP("ui_gen/button_3_down.bmp");
	button_images[8] = SDL_LoadBMP("ui_gen/button_4_up.bmp");
	button_images[9] = SDL_LoadBMP("ui_gen/button_4_down.bmp");

	redraw_stroke_sample();
	draw_buttons_to_bg();
}

void destroyBrushEditor() {
	int i;
	for(i=0;i<6;++i) {
		SDL_FreeSurface(slider_images[i]);
	}
	for(i=0;i<8;++i) {
		SDL_FreeSurface(button_images[i]);
	}

	SDL_FreeSurface(bg);
	SDL_FreeSurface(slider);
	SDL_FreeSurface(handle);
	SDL_FreeSurface(preview_bg);
}

void renderBrushEditor(SDL_Surface *target, UI_AREA *area) {
		int i;
		SDL_Rect spos;

		SDL_BlitSurface(bg,NULL,target,area);

		//our slider images are 30x244 and we have a
		// 400x256 area to fill with sliders

		for(i=0; i<6; ++i) {
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

void brusheditor_mousedown(int x,int y, UI_AREA *area) {
		int idx = (x-10) / 35;
		int vert = (y-6);

		//we're in the margins
		if(idx<0) {
				return;
		}

		sVals[idx] = (double)vert / 244.0;
		if(sVals[idx]>1) sVals[idx]=1;
		if(sVals[idx]<0) sVals[idx]=0;
		sVals[idx] = 1-sVals[idx];

		redraw_stroke_sample();
}
