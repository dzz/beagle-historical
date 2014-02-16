#include "colorPicker.h"
#include "panels.h"
#include <SDL.h>
#include <math.h>
#include "../drawing/drawingSurfaces.h"

static int wheel_x =-1;
static int wheel_y =-1;

static int sv_x = -1;
static int sv_y = -1;

typedef struct {
		double h;
		double s;
		double v;
} hsv;

static hsv primary;
static hsv secondary;

static cp_color primary_rgb;
static cp_color secondary_rgb;

static double h = 0;
static double s = 0;
static double v = 0;

cp_color cur_color;

unsigned int secondary_toggle = 0;

void cp_toggle_primary_secondary(void) {
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
}

cp_color rgb_from_dbl(double r,double g, double b) {
	cp_color ret;
	ret.r = (unsigned char)(r*255);
	ret.g = (unsigned char)(g*255);
	ret.b = (unsigned char)(b*255);
	ret.a = 255;
	return ret;
}
cp_color rgb_from_hsv(double h, double s, double v)
{
    double c = 0.0, m = 0.0, x = 0.0;
    {
        c = v * s;
        x = c * (1.0 - fabs(fmod(h / 60.0, 2) - 1.0));
        m = v - c;
        if (h >= 0.0 && h < 60.0)
        {
            return rgb_from_dbl(c + m, x + m, m);
        }
        else if (h >= 60.0 && h < 120.0)
        {
            return rgb_from_dbl(x + m, c + m, m);
        }
        else if (h >= 120.0 && h < 180.0)
        {
            return rgb_from_dbl(m, c + m, x + m);
        }
        else if (h >= 180.0 && h < 240.0)
        {
            return rgb_from_dbl(m, x + m, c + m);
        }
        else if (h >= 240.0 && h < 300.0)
        {
            return rgb_from_dbl(x + m, m, c + m);
        }
        else if (h >= 300.0 && h < 360.0)
        {
            return rgb_from_dbl(c + m, m, x + m);
        }
        else
        {
            return rgb_from_dbl(m, m, m);
        }
    }
    return rgb_from_dbl(0,0,0);
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
		double r = s*s;
		double hr = ((h+180)/180)*3.14;
		double wx = ((cos(hr)*r)+1.0)/2.0;
		double wy = ((sin(hr)*r)+1.0)/2.0;
		wheel_x = (int)(wx * COLORPICKER_WIDTH); 
		wheel_y = (int)(wy * (COLORPICKER_HEIGHT / 2));

		sv_x = (int)(s*COLORPICKER_WIDTH);
		sv_y = ((int)((1-v)*(COLORPICKER_HEIGHT/2)))+(COLORPICKER_HEIGHT/2);
	}
}

static SDL_Surface * interfaceSurface;
static SDL_Surface * if_buffer;

void drawSVTriangle(int w,int hgt) {
	int h2 = hgt/2;
	int h4 = hgt/4;
	int w2 = w/2;
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
	SDL_Surface *bg = SDL_LoadBMP("cp_bg.bmp");

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
				double h = atan2(unit_y,unit_x)*(350/(3.14*2))+180;

				if(h == 0) h = 0.0001;
				if(h == 360) h = 359.999;
				coord = (y*w)+x;
				if(d<1) {
					cp_color rgb = rgb_from_hsv(h,sqrt(d),1);
					pixels[coord] = SDL_MapRGB(
									interfaceSurface->format,
									rgb.r,rgb.g,rgb.b);
				}
			}
	SDL_UnlockSurface(interfaceSurface);
}


void initColorPicker(void) {
		cur_color.r=100;
		cur_color.g=128;
		cur_color.b=64;
		cur_color.a=255;
		interfaceSurface = createDrawingSurface(COLORPICKER_WIDTH,COLORPICKER_HEIGHT);
		if_buffer = createDrawingSurface(COLORPICKER_WIDTH,COLORPICKER_HEIGHT);
		drawColorWheel(COLORPICKER_WIDTH,COLORPICKER_HEIGHT);
		drawSVTriangle(COLORPICKER_WIDTH,COLORPICKER_HEIGHT);
}


cp_color getCurColor(void) {
	return rgb_from_hsv(h,s,v);
}

cp_color getPrimaryColor(void) {
	return primary_rgb;
}

cp_color getSecondaryColor(void) {
	return secondary_rgb;
}

void renderColorPicker(SDL_Surface *target, UI_AREA *area) {
		int x_start;
		int x_end;
		int y_start;
		int y_end;

		if(area->x > target->w) return;
		if(area->y > target->h) return;

		x_start = area->x;
		x_end = area->x + area->w;
		y_start = area->y;
		y_end = area->y + area->h;

		x_end = x_end > target->w ? target->w : x_end;
		y_end = y_end> target->h ? target->h : y_end;

		SDL_BlitSurface(interfaceSurface,NULL,if_buffer,NULL);

		if(wheel_x !=-1 ) {
			int rad = 5;
			int t = 1;
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
		}

		SDL_BlitSurface(if_buffer,NULL,target,(SDL_Rect *)area);
}

void destroyColorPicker(void) {
		SDL_FreeSurface(interfaceSurface);
		SDL_FreeSurface(if_buffer);
}

void colorpicker_mousedown(int x,int y, UI_AREA *area) {
	int h2 = COLORPICKER_HEIGHT/2;
	int w2 = COLORPICKER_WIDTH/2;
	int h4 = h2/2;
	if(y<h2) {
			double unit_x = ((double)x - (double)w2)/(double)w2;
			double unit_y = ((double)y - (double)h4)/(double)h4;
			double d= sqrt(((unit_x)*(unit_x))+((unit_y)*(unit_y)));
			h = atan2(unit_y,unit_x)*(350/(3.14*2))+180;
			if(h<0.0001) h = 0.0001;
			if(h>359.999) h = 359.999;
			s = sqrt(d);

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
