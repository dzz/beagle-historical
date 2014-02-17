#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "brush.h"
#include "../user/stylus.h"
#include "../user/colorPicker.h"
#include "../user/brushEditor.h"
#include "../system/dirty.h"
#include "../colors/pixmap.h"
#include "../compositor/compositor.h"

static SDL_Surface* gContext;

static float brush_size;
static double brush_power; // "hardness"
static float brush_alpha;
static int brush_mixpaint = 0;
static double brush_jitter = 0;

void brush_setValuesFromUI() {
	const double brush_min = 5.0;
	const double brush_max = 125.0;
	const double brush_pow_min = 0.05;
	const double brush_pow_max = 128.0;

	brush_size = (float)(brush_min+((brush_max-brush_min) * get_brusheditor_value(0)));
	brush_power = (float)(brush_pow_min+((brush_pow_max-brush_pow_min) * get_brusheditor_value(1)));
	brush_alpha = get_brusheditor_value(2);
	brush_mixpaint = get_brusheditor_toggle(0);
	brush_jitter = get_brusheditor_value(4);
}

void initBrush( SDL_Surface* context ) {
	gContext = context;
}


void brushPaint(stylusState a, stylusState b) {
	brush_setValuesFromUI();
	brush_drawStrokeSegment(a.x,a.y,b.x,b.y,(float)a.pressure,(float)b.pressure, gContext);
}

unsigned char mix_char(unsigned char l, unsigned char r, unsigned char idx) {
/*	float blend = (float)idx/255;

	float final = (float)l * blend + (float)r * (1-blend);
	return (unsigned char)final; */

	unsigned int yolo = (l*idx+r*(255-idx))/255;
	return yolo;
}

pixMap mixed;
unsigned int*  mix(pixMap src, pixMap dst) {
	const int Fringe = 0;
	unsigned int alpha;
/*
	if(dst.p.a == 0 && Fringe == 0) {
		return &src.pix;
	}
	*/

	mixed.p.r = mix_char(src.p.r,dst.p.r,src.p.a);
	mixed.p.g = mix_char(src.p.g,dst.p.g,src.p.a);
	mixed.p.b = mix_char(src.p.b,dst.p.b,src.p.a);

	if(src.p.a > dst.p.a) {
			alpha = src.p.a;
	}
	else {
    	alpha = src.p.a+dst.p.a;
		if(alpha>255)
				alpha = 255;

	}

	mixed.p.a = (unsigned char)alpha;
	return &mixed.pix;
	//return &mixed.pix;
}


float map_intensity(float x,float y,float p) {
		double d = sqrtf((x*x)+(y*y));
		if(d>1) return 0; else return ((1-d))*(brush_power+(brush_power*p));
}

void getMixedPaint(pixMap *pix, float p) {
		cp_color prim = getPrimaryColor();
		if( brush_mixpaint == 0) {
				pix->p.r = prim.r;
				pix->p.g = prim.g;
				pix->p.b = prim.b;
		} else {
				cp_color secon = getSecondaryColor();
				pix->p.r = (unsigned char)((float)prim.r * p + (float)secon.r * (1-p));
				pix->p.g = (unsigned char)((float)prim.g * p + (float)secon.g * (1-p));
				pix->p.b = (unsigned char)((float)prim.b * p + (float)secon.b * (1-p));
		}
}

void plotSplat(int x, int y, int r, float p, SDL_Surface* ctxt) {
	signed int i;
	signed int j;
	signed int r2 = r+r;
	const float pi = 2;
	const float piov_2 = 1;
	float delta = pi / (float)(r2);
	float plotX = -piov_2;
	float plotY = -piov_2;
	float intensity;
	unsigned char v = 0;
	int coord = (x-r)+((y-r) * ctxt->w);
	unsigned int jmp = (ctxt->w - ((r*2)));
	unsigned int ucoord;
	pixMap dest;
	pixMap current;
	unsigned int* pixels = (unsigned int*)ctxt->pixels;
	int clipped_x = ctxt->w -(x+r);
	int end = ctxt->w*ctxt->h;
   	clipped_x = clipped_x	< 0 ? r2 + clipped_x : r2;

	getMixedPaint(&current,p);

	for( i=0; i<clipped_x; ++i) {
		for( j=0; j<r2; ++j) {
			plotX += delta;
			if(coord>0 && coord<end) {
					ucoord = (unsigned int)coord;
					intensity = map_intensity(plotX,plotY,p);
					v = (unsigned char)(intensity*brush_alpha);
					dest.pix =pixels[ucoord];
					current.p.a = v;
					pixels[ucoord] = *mix(current,dest);
			}
			coord++;
		}
		plotX = -piov_2;
		plotY += delta;
		coord += jmp;
	}
	//if we're rendering directly to the active drawing context,
	//invalidate the dirty rect. Othewise, we're in use somewhere
	//else not tied to the global dirty rect manager
	if(ctxt == gContext) {
		invalidateDirty(x-r,y-r,x+r,y+r);
	}
}

void brushReset() {
}

void brush_drawStrokeSegment(int x0, int y0, int x1, int y1,float p0,float p1, SDL_Surface* ctxt) {

	int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
   	int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
   	int err = (dx>dy ? dx : -dy)/2, e2;

		SDL_LockSurface(ctxt);
    	for(;;){
				double j_size = 100*brush_jitter*p1;
				double j_x = ((double)rand() / RAND_MAX)*brush_jitter*j_size;
				double j_y = ((double)rand() / RAND_MAX)*brush_jitter*j_size;

				plotSplat((x0+j_x),(y0+j_y),(int)(p0*p0*brush_size),p1, ctxt);
    			if (x0==x1 && y0==y1) break;
    			e2 = err;
    			if (e2 >-dx) { err -= dy; x0 += sx; }
    			if (e2 < dy) { err += dx; y0 += sy; }
    	}
		SDL_UnlockSurface(ctxt);
}

unsigned int mix_noalpha(cp_color src, cp_color dst,unsigned char blend) {
	pixMap mixed;

	mixed.p.r = comp_alpha_over(src.r,dst.r,blend);
	mixed.p.g = comp_alpha_over(src.g,dst.g,blend);
	mixed.p.b = comp_alpha_over(src.b,dst.b,blend);

	return mixed.pix;
}
