#include <SDL.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "../../colors/colors.h"

#include "../../drawing/drawingSurfaces.h"

#include "../panels.h"

#include "colorPicker.h"
#include "brushEditor.h"




#define EDITING_COLOR 1
#define NOT_EDITING_COLOR 0

static void commitColor(void);

static int wheel_x =-1;
static int wheel_y =-1;
static int buffered_wheel_x = -1;
static int buffered_wheel_y = -1;
static int sv_x = -1;
static int sv_y = -1;
static int buffered_sv_x = -1;
static int buffered_sv_y = -1;

static hsv primary;
static hsv secondary;

static cp_color primary_rgb;
static cp_color secondary_rgb;

static double h = 0;
static double s = 0;
static double v = 0;

cp_color cur_color;

static unsigned int secondary_toggle = 0;

unsigned int get_cp_secondary(void) {
	return secondary_toggle;
}

void drawSVTriangle(int w,int hgt);
void drawColorWheel(int w,int hgt);

void cp_toggle_primary_secondary(void) {
	cp_color tmpc;
	int tmp;
	tmp = wheel_x; wheel_x = buffered_wheel_x; buffered_wheel_x = tmp;
	tmp = wheel_y; wheel_y = buffered_wheel_y; buffered_wheel_y = tmp;
	tmp = sv_x; sv_x = buffered_sv_x; buffered_sv_x = tmp;
	tmp = sv_y; sv_y = buffered_sv_y; buffered_sv_y = tmp;

	secondary_toggle = !secondary_toggle;
	if(secondary_toggle == 1) {
			h = secondary.h;
			s = secondary.s;
			v = secondary.v;
	} else {
			h = primary.h;
			s = primary.s;
			v = primary.v;
	}
	commitColor();
	drawSVTriangle(COLORPICKER_WIDTH,COLORPICKER_HEIGHT);
}

static void commitColor(void) {
	if(secondary_toggle == 1) {
		secondary.h = h;
		secondary.s = s;
		secondary.v = v;
	} else {
		primary.h = h;
		primary.s = s;
		primary.v = v;
	}
	primary_rgb = rgb_from_hsv(primary.h,primary.s,primary.v);
	secondary_rgb = rgb_from_hsv(secondary.h,secondary.s,secondary.v);

	{
		double r = 0.6;
		double hr = ((h+180)/180)*M_PI;
		double wx = ((cos(hr)*r)+1.0)/2.0;
		double wy = ((sin(hr)*r)+1.0)/2.0;
		wheel_x = (int)(wx * COLORPICKER_WIDTH);
		wheel_y = (int)(wy * (COLORPICKER_HEIGHT / 2));

		sv_x = (int)(s*COLORPICKER_WIDTH);
		sv_y = ((int)((1-v)*(COLORPICKER_HEIGHT/2)))+(COLORPICKER_HEIGHT/2);
	}
	brusheditor_redraw_stroke();
}


static SDL_Surface * interfaceSurface;
static SDL_Surface * if_buffer;

void drawSVTriangle(int w,int hgt) {
	int h2 = hgt/2;
	int x =0;
	int coord;
	int y =0;
	unsigned int* pixels;
	SDL_LockSurface(interfaceSurface);
	pixels = (unsigned int*)interfaceSurface->pixels;

	for(x = 0; x<w;++x) {
		for(y = h2+1; y<hgt; ++y) {
			double unit_y = (double)(y-h2)/(double)h2;
			double unit_x = (double)(x)/(double)(w);
			coord = (y*w)+x;
			{
					cp_color rgb = rgb_from_hsv(h,unit_x,(1-unit_y));
					pixels[coord] = SDL_MapRGB(
									interfaceSurface->format,
									rgb.r,rgb.g,rgb.b);
			}
		}
	}
	SDL_UnlockSurface(interfaceSurface);
}

void drawColorWheel(int w,int h) {
	SDL_Surface *bg = SDL_LoadBMP("ui_gen/generated/cp_bg.bmp");

	int h2 = h/2;
	int h4 = h/4;
	int w2 = w/2;
	int x =0;
	int coord;
	int y =0;
	unsigned int* pixels;
	SDL_BlitSurface(bg,NULL,interfaceSurface,NULL);
	SDL_FreeSurface(bg);
	SDL_LockSurface(interfaceSurface);
	pixels = (unsigned int*)interfaceSurface->pixels;
	for(x=0; x<w; x++)
			for(y=0; y<h2; ++y) {
				double unit_x = ((double)x - (double)w2)/(double)w2;
				double unit_y = ((double)y - (double)h4)/(double)h4;
				double d= sqrt(((unit_x)*(unit_x))+((unit_y)*(unit_y)));
				double calc_h = atan2(unit_y,unit_x)*(350/(M_PI*2))+180;

				if(h == 0) h = 0.0001;
				if(h == 360) h = 359.999;
				coord = (y*w)+x;
				if(d<0.78 && d>0.42) {

					cp_color rgb = rgb_from_hsv(calc_h,s,v);
					pixels[coord] = SDL_MapRGB(
									interfaceSurface->format,
									rgb.r,rgb.g,rgb.b);
				}
			}
	SDL_UnlockSurface(interfaceSurface);
}

void randomizeColor(int seed) {
		srand(seed);
		h = rand()%360;
		s = (double)rand()/RAND_MAX;
		v = (double)rand()/RAND_MAX;

		if(s<0.5) s = 0.5;
		if(v<0.5) v = 0.5;
}

void initColorPicker(void) {
		int seed = time(0) * 256;

		cur_color.r=100;
		cur_color.g=128;
		cur_color.b=64;
		cur_color.a=255;


		interfaceSurface = createDrawingSurface(COLORPICKER_WIDTH,COLORPICKER_HEIGHT);
		if_buffer = createDrawingSurface(COLORPICKER_WIDTH,COLORPICKER_HEIGHT);


		s = 1;
		v = 1;
		drawColorWheel(COLORPICKER_WIDTH,COLORPICKER_HEIGHT);


		randomizeColor(seed);
		commitColor();
		cp_toggle_primary_secondary();

		randomizeColor(seed+1024);
		commitColor();

		cp_toggle_primary_secondary();
}


cp_color getCurColor(void) {
	return rgb_from_hsv(h,s,v);
}

void setCurColor(uint_rgba_map color) {
	hsv_from_rgb(color, &h,&s,&v);
	commitColor();
}

cp_color getPrimaryColor(void) {
	
	return primary_rgb;
}

cp_color getSecondaryColor(void) {
	return secondary_rgb;
}

void renderColorPicker(SDL_Surface *target, UI_AREA *area) {

		int x_end;

		int y_end;

		if(area->x > target->w) return;
		if(area->y > target->h) return;

		x_end = area->x + area->w;

		y_end = area->y + area->h;

		x_end = x_end > target->w ? target->w : x_end;
		y_end = y_end> target->h ? target->h : y_end;

		SDL_BlitSurface(interfaceSurface,NULL,if_buffer,NULL);
		
		{
			SDL_Rect r;
			int rad = 12;
			r.x = (COLORPICKER_WIDTH / 2) - rad;
			r.w = rad;
			r.h = rad*2;
			r.y = (COLORPICKER_HEIGHT / 4) - rad;

			SDL_FillRect(if_buffer,&r,SDL_MapRGB(if_buffer->format, primary_rgb.r,primary_rgb.g,primary_rgb.b));
			r.x+=rad;	
			SDL_FillRect(if_buffer,&r,SDL_MapRGB(if_buffer->format, secondary_rgb.r,secondary_rgb.g,secondary_rgb.b));
			 

		}

		if(wheel_x !=-1 ) {
			int rad = 7;
			SDL_Rect r;
			r.x = (wheel_x-rad);
			r.y = (wheel_y-rad);
			r.w = rad*2;
			r.h = rad*2;
			SDL_FillRect(if_buffer,&r,SDL_MapRGB(if_buffer->format, 255,255,255));

			r.x = (sv_x-rad);
			r.y = (sv_y-rad);
			r.w = rad*2;
			r.h = rad*2;
			SDL_FillRect(if_buffer,&r,SDL_MapRGB(if_buffer->format, 255,255,255));

			rad -=2;
			r.x+=2;
			r.y+=2;
			r.w = rad*2;
			r.h = rad*2;

			if(secondary_toggle==0)	
				SDL_FillRect(if_buffer,&r,SDL_MapRGB(if_buffer->format, primary_rgb.r,primary_rgb.g,primary_rgb.b));
			else
				SDL_FillRect(if_buffer,&r,SDL_MapRGB(if_buffer->format, secondary_rgb.r,secondary_rgb.g,secondary_rgb.b));

		}


		if(buffered_wheel_x !=-1 ) {
			int rad = 4;
			SDL_Rect r;
			r.x = (buffered_wheel_x-rad);
			r.y = (buffered_wheel_y-rad);
			r.w = rad*2;
			r.h = rad*2;
			SDL_FillRect(if_buffer,&r,SDL_MapRGB(if_buffer->format, 0,0,0));

			r.x = (buffered_sv_x-rad);
			r.y = (buffered_sv_y-rad);
			r.w = rad*2;
			r.h = rad*2;
			SDL_FillRect(if_buffer,&r,SDL_MapRGB(if_buffer->format, 0,0,0));

			rad -=1;
			r.x+=1;
			r.y+=1;
			r.w = rad*2;
			r.h = rad*2;

			if(secondary_toggle==1)	
				SDL_FillRect(if_buffer,&r,SDL_MapRGB(if_buffer->format, primary_rgb.r,primary_rgb.g,primary_rgb.b));
			else
				SDL_FillRect(if_buffer,&r,SDL_MapRGB(if_buffer->format, secondary_rgb.r,secondary_rgb.g,secondary_rgb.b));
		}

		SDL_BlitSurface(if_buffer,NULL,target,(SDL_Rect *)area);
}

void destroyColorPicker(void) {
		SDL_FreeSurface(interfaceSurface);
		SDL_FreeSurface(if_buffer);
}

static int editing = NOT_EDITING_COLOR;
void colorpicker_mousemotion(int x,int y, UI_AREA *area) {
	if(editing == EDITING_COLOR) {
		colorpicker_mousedown(x,y,area);
	}
}

void colorpicker_mouseup(int x,int y, UI_AREA *area) {
	editing = NOT_EDITING_COLOR;
}

void colorpicker_color_selection_mousedown(int x,int y, UI_AREA *area) {
	int h2 = COLORPICKER_HEIGHT/2;
	int w2 = COLORPICKER_WIDTH/2;
	int h4 = h2/2;

	editing = EDITING_COLOR;
	if(y<h2) {
			double unit_x = ((double)x - (double)w2)/(double)w2;
			double unit_y = ((double)y - (double)h4)/(double)h4;
			h = atan2(unit_y,unit_x)*(350/(M_PI*2))+180;
			if(h<0) h = 0;
			if(h>360) h =360; 

	} else {
		y-=h2;
		{
				double unit_x = (double)x/COLORPICKER_WIDTH;
				double unit_y = (double)y/(double)h2;
				s = unit_x;
				v = 1-unit_y;
		}
	}
	commitColor();
	drawSVTriangle(COLORPICKER_WIDTH,COLORPICKER_HEIGHT);
}

void colorpicker_mousedown(int x,int y, UI_AREA *area) {
		if( (x> (64-12)) && (y>(64-12)) &&
		  (x<(64+12)) &&  (y<64+12)) {
			cp_toggle_primary_secondary();
		} else{
			   	colorpicker_color_selection_mousedown(x,y,area);
		}
}

void colorpicker_mouseleave(){
		editing = NOT_EDITING_COLOR;
}