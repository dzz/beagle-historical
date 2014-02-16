#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <math.h>

#include "../user/stylus.h"
#include "../user/colorPicker.h"
#include "../system/dirty.h"
#include "../colors/pixmap.h"
#include "../compositor/compositor.h"

static SDL_Surface* gContext;

void initBrush( SDL_Surface* context ) {
	gContext = context;
}

void drawLine(int x0,int y0,int x1,int y1,float p0,float p1);

void brushPaint(stylusState a, stylusState b) {
	int tlX,tlY,brX,brY;
	SDL_Rect rect;

	rect.x = a.x;
	rect.y = a.y;
	rect.w = (int)(a.pressure*16);
	rect.h = (int)(a.pressure*16);

	drawLine(a.x,a.y,b.x,b.y,(float)a.pressure,(float)b.pressure);

	tlX = MIN(a.x,b.x);
	tlY = MIN(a.y,b.y);
	brX = MAX(a.x,b.x);
	brY = MAX(a.y,b.y);

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
		float d = sqrtf((x*x)+(y*y));
		if(d>1) return 0; else return ((1-d))*(32.0f+(32.0f*p));
}

void getMixedPaint(pixMap *pix, float p) {
	cp_color prim = getPrimaryColor();
	cp_color secon = getSecondaryColor();
	pix->p.r = (unsigned char)((float)prim.r * p + (float)secon.r * (1-p));
	pix->p.g = (unsigned char)((float)prim.g * p + (float)secon.g * (1-p));
	pix->p.b = (unsigned char)((float)prim.b * p + (float)secon.b * (1-p));
}

void plotSplat(int x, int y, int r, float p) {
	signed int i;
	signed int j;
	signed int r2 = r+r;
	float pi = 2;
	float piov_2 = 1;
	float delta = pi / (float)(r2);
	float plotX = -piov_2;
	float plotY = -piov_2;
	float intensity;
	float hardness = 1;
	unsigned char v = 0;
	int coord = (x-r)+((y-r) * gContext->w);
	unsigned int jmp = (gContext->w - ((r*2)));
	unsigned int ucoord;
	pixMap dest;
	pixMap current;
	unsigned int* pixels = (unsigned int*)gContext->pixels;
	int clipped_x = gContext->w -(x+r);
   	clipped_x = clipped_x	< 0 ? r2 + clipped_x : r2;

	getMixedPaint(&current,p);

	for( i=0; i<clipped_x; ++i) {
		for( j=0; j<r2; ++j) {
			plotX += delta;
			if(coord>0) {
					ucoord = (unsigned int)coord;
					intensity = map_intensity(plotX,plotY,p);
					v = (unsigned char)intensity;
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
	invalidateDirty(x-r,y-r,x+r,y+r);
}

void plotBrush(int x,int y,float p)
{
  SDL_Rect rect;
  unsigned char col = (unsigned char)((p)*255);
  unsigned char col2 = col/2;
  rect.x=x;
  rect.y=y;
  rect.w=1+(int)(128*(p*p));
  rect.h=1+(int)(72*(p*p));

  SDL_FillRect(gContext, &rect, SDL_MapRGB( gContext->format, col, 255-col, col ) );
  invalidateDirty(rect.x,rect.y,rect.x+rect.w,rect.y+rect.h);

  rect.w -= 2;
  rect.h -= 2;
  rect.x ++;
  rect.y ++;

  SDL_FillRect(gContext, &rect, SDL_MapRGB( gContext->format, col2, col2, col2 ) );
}


void brushReset() {
}

void drawLine(int x0, int y0, int x1, int y1,float p0,float p1) {

	int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
   	int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
   	int err = (dx>dy ? dx : -dy)/2, e2;

		SDL_LockSurface(gContext);
    	for(;;){
				plotSplat(x0,y0,(int)(p0*p0*30),p1);
    			if (x0==x1 && y0==y1) break;
    			e2 = err;
    			if (e2 >-dx) { err -= dy; x0 += sx; }
    			if (e2 < dy) { err += dx; y0 += sy; }
    	}
		SDL_UnlockSurface(gContext);
}

unsigned int mix_noalpha(cp_color src, cp_color dst,unsigned char blend) {
	pixMap mixed;

	mixed.p.r = comp_alpha_over(src.r,dst.r,blend);
	mixed.p.g = comp_alpha_over(src.g,dst.g,blend);
	mixed.p.b = comp_alpha_over(src.b,dst.b,blend);

	return mixed.pix;
}
