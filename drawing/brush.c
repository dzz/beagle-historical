#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#include <SDL.h>
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
static float brush_size_base;
static double brush_power; 
static float brush_alpha;
static int brush_mixpaint = 0;
static int brush_erase = 0;
static double brush_jitter = 0;
static double brush_noise = 0;
static double brush_pressure_dynamics = 0;

unsigned char (*active_mixing_function)(unsigned char,unsigned char,unsigned char);

 unsigned char mix_char(unsigned char l, unsigned char r, unsigned char idx);
 unsigned char bright_char(unsigned char l, unsigned char r, unsigned char idx);
 unsigned char dark_char(unsigned char l, unsigned char r, unsigned char idx);

void brush_setValuesFromUI() {
	const double brush_min = 0.2;
	const double brush_max = 255.0;
	const double brush_pow_min = 0.01;
	const double brush_pow_max = 1;
	const double brush_jitter_max = 4; 

	brush_size_base = get_brusheditor_value(0);
	brush_size = (float)(brush_min+((brush_max-brush_min) * brush_size_base));

	brush_power = (float)(brush_pow_min+((brush_pow_max-brush_pow_min) * get_brusheditor_value(1)));
	brush_alpha = get_brusheditor_value(2);
	brush_pressure_dynamics = get_brusheditor_value(3);
	brush_jitter = get_brusheditor_value(4)*brush_jitter_max;
	brush_noise = get_brusheditor_value(5);

	brush_mixpaint = get_brusheditor_toggle(0);
	brush_erase = get_brusheditor_toggle(1);

	switch(get_brusheditor_radio()) {
		case 1:
			active_mixing_function=&mix_char;
			break;
		case 2:
			active_mixing_function=&bright_char;
			break;
		case 3:
			active_mixing_function=&dark_char;
			break;
	}

}

void initBrush( SDL_Surface* context ) {
	int i;

	active_mixing_function = &mix_char;
	gContext = context;
}


void brushPaint(stylusState a, stylusState b) {
	brush_drawStrokeSegment(a.x,a.y,b.x,b.y,(float)a.pressure,(float)b.pressure, gContext);
}

unsigned char mix_char(unsigned char l, unsigned char r, unsigned char idx) {
	return (((l*idx+r*((255-idx)))/255));
}

unsigned char bright_char(unsigned char l, unsigned char r, unsigned char idx) {
		unsigned int yolo = ((l*idx)+(r*255))/255;
		if(yolo>255) yolo=255;
		return yolo;
}

unsigned char dark_char(unsigned char l, unsigned char r, unsigned char idx) {

		float a = (float)l/255;
		float b = (float)r/255;
		float c = (float)idx/255;

		float mix_amt = 1-c;
		float mul_v = a*b;

		float result = (mul_v*c) + (mix_amt*b);

		unsigned int yolo = (unsigned int)( (result) * 255);
		return yolo;
}

pixMap mixed;

unsigned int* erase(pixMap src,pixMap dst) {

	mixed.p.r = dst.p.r;
	mixed.p.g = dst.p.g;
	mixed.p.b = dst.p.b;

	if(dst.p.a<src.p.a) mixed.p.a = 0; else mixed.p.a=dst.p.a-src.p.a;
	return &mixed.pix;
}

unsigned int*  mix(pixMap src, pixMap dst) {
	unsigned int alpha;

	mixed.p.r = (*active_mixing_function)(src.p.r,dst.p.r,src.p.a);
	mixed.p.g = (*active_mixing_function)(src.p.g,dst.p.g,src.p.a);
	mixed.p.b = (*active_mixing_function)(src.p.b,dst.p.b,src.p.a);

	//this seemed right but gives undesirable results (weird overflow-esque
	//artifacts when compositing brushes on blank canavses. Without the first
	//block we get a "wet edges" effect due to gradiating to the drawing surface's
	//base colour.
	//
	//will this out correctly at some point

	/*
	if(src.p.a > dst.p.a) {
			alpha = src.p.a;
	}
	else
	*/
	{
    	alpha = src.p.a+dst.p.a;
		if(alpha>255)
				alpha = 255;

	}
	mixed.p.a = (unsigned char)alpha;
	return &mixed.pix;
	//return &mixed.pix;
}

__inline float squareRoot(float x)
{
  unsigned int i = *(unsigned int*) &x;

  // adjust bias
  i  += 127 << 23;
  // approximation of square root
  i >>= 1;

  return *(float*) &i;
}


__inline float map_intensity(float x,float y,float p) {
		float d = 1 - squareRoot((x*x)+(y*y));


		if( d<0) return 0;
		d *= 1 / brush_power;
		if(d>1) d = 1;
		return d*255;
		
/*		 d = squareRoot((x*x)+(y*y));

		if(d>1) return 0; else return ((1-d))*(brush_power*p*(1+brush_pressure_dynamics));

		//return 255;*/
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

int g_seed = 0;

__inline int fastrand() { 
  g_seed = (214013*g_seed+2531011); 
  return (g_seed>>16)&0x7FFF; 
} 

__inline void plotSplat(int x, int y, int r, float p, SDL_Surface* ctxt) {
		signed int i;
		signed int j;
		const signed int r2 = r+r;
		const float pi = 2;
		const float piov_2 = 1;
		const float delta = pi / (float)(r2);
		float plotX = -piov_2;
		float plotY = -piov_2;
		float intensity;
		unsigned char v = 0;
		int coord = (x-r)+((y-r) * ctxt->w);
		const unsigned int jmp = (ctxt->w - ((r*2)));
		unsigned int ucoord;
		pixMap dest;
		pixMap current;
		unsigned int* pixels = (unsigned int*)ctxt->pixels;
		int clipped_x = ctxt->w -(x+r);
		const int end = ctxt->w*ctxt->h;
		float noise = 1;
		unsigned int* (*mixer)(pixMap,pixMap);
		float buf;

		clipped_x = clipped_x < 0 ? r2 + clipped_x : r2;

		if(brush_erase == 1) {
				mixer = &erase;
		} else {
				mixer = &mix;
		}

		getMixedPaint(&current,p);

		for( i=0; i<clipped_x; ++i) {
				for( j=0; j<r2; ++j) {
						plotX += delta;
						if(( x + (j-r) ) < (signed int)ctxt->w)
						{
								noise = 1-(((float)fastrand()/RAND_MAX)*brush_noise);

								if(coord>0 && coord<end) {
										intensity = map_intensity(plotX,plotY,p);
										buf = intensity*brush_alpha*noise;
										v = (unsigned char)(buf);
										dest.pix =pixels[coord];
										current.p.a = v;
										pixels[coord] = *(*mixer)(current,dest);
								}
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
		float pD = brush_pressure_dynamics;

		double dX = (x1-x0);
		double dY = (y1-y0);
		double dP = (p1-p0);
		double len = squareRoot(dX*dX+dY*dY);

		int iLen = (int)len;
		int i = 0;

		double x = x0;
		double y = y0;

		dX/=len;
		dY/=len;
		dP/=len;

		SDL_LockSurface(ctxt);
		for(i=0; i< iLen; ++i) {
				int radius;	
				x+=dX;
				y+=dY;
				p0+=dP;
 				radius = (int)((p0*pD)*brush_size);
				plotSplat((int)x,(int)y,radius,p0, ctxt);
		}
		SDL_UnlockSurface(ctxt);
}

void brush_drawStrokeSegment_fast(int x0, int y0, int x1, int y1,float p0,float p1, SDL_Surface* ctxt) {
		int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
		int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
		int err = (dx>dy ? dx : -dy)/2, e2;

		int space_ctr = 0;

		float pD = brush_pressure_dynamics;

		SDL_LockSurface(ctxt);
		for(;;){
				

				int radius;
				int spacing = 
						(radius > 24 ) ?	
						4 + ( (radius*radius) / 255 ) :
						1;

				if (x0==x1 && y0==y1) break;
				e2 = err;
				if (e2 >-dx) { err -= dy; x0 += sx; }
				if (e2 < dy) { err += dx; y0 += sy; }

 				radius = (int)((p0*pD)*brush_size);

				if(space_ctr==0){
						int jtr_x = (((float)fastrand()) / RAND_MAX ) * (radius*brush_jitter);
						int jtr_y = (((float)fastrand()) / RAND_MAX ) * (radius*brush_jitter);
						plotSplat((x0+jtr_x),(y0+jtr_y),radius,p0, ctxt);
				}
				space_ctr = (space_ctr+1) % spacing;
		}
		SDL_UnlockSurface(ctxt);
}
