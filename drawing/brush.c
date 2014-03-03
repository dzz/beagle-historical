//  brush engine specific defines:
//
//  	#define BRUSH_SPEED_HACK   	- not a great speed hack, but skips some
//  								bilinear interp for dabs
//
//  	#define BRUSH_FANCY			- secret noise and dithering
//
//      #ifdef BRUSH_INTENSITY_TO_COLOR_MAPPING
//      							- looks cool but needs work to be useful


//#define BRUSH_INTENSITY_TO_COLOR_MAPPING
//#define BRUSH_SPEED_HACK
#define BRUSH_FANCY

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
#include "../user/mapperEditorBank.h"
#include "../system/dirty.h"
#include "../colors/pixmap.h"
#include "../compositor/compositor.h"

static SDL_Surface* gContext;

static float brush_alpha_mod;
static float brush_size_mod;
static double brush_jitter_mod = 0;
static double brush_noise_mod = 0;
static double brush_color_mix_mod = 0;

static float brush_size_base;
static double brush_jitter_base = 0;
static double brush_noise_base = 0;

static int brush_dab_index = 0;
static int brush_mixpaint = 0;
static int brush_erase = 0;
unsigned static int brush_mix_mode = 0;
static int brush_loaded_dabs = 0;

unsigned char (*active_mixing_function)(unsigned char,unsigned char,unsigned char);

unsigned char mix_char(unsigned char l, unsigned char r, unsigned char idx);
unsigned char bright_char(unsigned char l, unsigned char r, unsigned char idx);
unsigned char dark_char(unsigned char l, unsigned char r, unsigned char idx);

void brush_toggleMixMode() {
	brush_mix_mode = !brush_mix_mode;
}

void brush_modulate_values(double pressure) {
		const double jitter_max = 8;
		brush_color_mix_mod = mapperbank_get_mapping(MAPPER_COLOR,pressure);
		brush_alpha_mod = mapperbank_get_mapping(MAPPER_ALPHA,pressure);
		brush_size_mod = brush_size_base * pow(mapperbank_get_mapping(MAPPER_SIZE,pressure),2);
		brush_jitter_mod = pow(mapperbank_get_mapping(MAPPER_JITTER,pressure),2) * jitter_max;
		brush_noise_mod = mapperbank_get_mapping(MAPPER_NOISE,pressure);
}

void brush_setValuesFromUI() {
	const double brush_min = 0.2;
	const double brush_max = 255.0;
	const double brush_pow_min = 0.01;
	const double brush_pow_max = 1;

	brush_size_base = (float)(brush_min+((brush_max-brush_min) * pow(get_brusheditor_value(0),2)));
	brush_dab_index = (int) (get_brusheditor_value(1) * brush_loaded_dabs);
	if(brush_dab_index>(brush_loaded_dabs-1))
		brush_dab_index = brush_loaded_dabs-1;

	brush_mixpaint = 1;
	brush_erase = 0;

	active_mixing_function=&mix_char;
}

#define MAX_DABS 32
double dabs[MAX_DABS][(64*64)+64];
SDL_Surface* dab_bmps[MAX_DABS];

SDL_Surface* brush_get_active_dab_bmp() {
	if(dab_bmps[brush_dab_index]!=0)
			return dab_bmps[brush_dab_index];
	else
			//hax
			return dab_bmps[0];
}

void initBrush( SDL_Surface* context ) {
	int i;
	int idx;

	for(idx = 0; idx< MAX_DABS; ++idx) {
			char dab_fname[1024];
			SDL_Surface *dabBmp;

			sprintf(dab_fname,"dabs/dab%02d.bmp",idx);
			dabBmp = SDL_LoadBMP(dab_fname);

			if(dabBmp == 0) {
				break;
			}
			dab_bmps[idx] = dabBmp;

			active_mixing_function = &mix_char;
			gContext = context;

			SDL_LockSurface(dabBmp);
			for(i=0; i<64*64;++i) {
					unsigned char* dabData = dabBmp->pixels;
					dabs[idx][i] = (double)255-dabData[i];
			}

			SDL_UnlockSurface(dabBmp);
			brush_loaded_dabs++;
	}
}

void destroyBrush() {
		int i;
		for( i=0; i< MAX_DABS; ++i) {
			if(dab_bmps[i]!=0) {
				SDL_FreeSurface(dab_bmps[i]);
			}
		}
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

		if(dst.p.a == 0) {
				dst.p.r = mixed.p.r;
				dst.p.g = mixed.p.g;
				dst.p.b = mixed.p.b;
		}

		mixed.p.r = (*active_mixing_function)(src.p.r,dst.p.r,src.p.a);
		mixed.p.g = (*active_mixing_function)(src.p.g,dst.p.g,src.p.a);
		mixed.p.b = (*active_mixing_function)(src.p.b,dst.p.b,src.p.a);

		{
				alpha = src.p.a+dst.p.a;
				if(alpha>255)
						alpha = 255;

		}
		mixed.p.a = (unsigned char)alpha;
		return &mixed.pix;

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

/*

	brush_power == photoshop "hardness"

__inline float map_intensity_square(float x,float y,float p) {
		float d = 1 - squareRoot((x*x)+(y*y));
		if( d<0) return 0;
		d *= 1 / brush_power;
		if(d>1) d = 1;
		return d*255;
}
*/

__inline float map_intensity(float x,float y,float p) {
		float  xc_d = (x*32)+32;
		float  yc_d = (y*32)+32;

		int xc=(int)xc_d;
		int yc=(int)yc_d;

#ifdef BRUSH_SPEED_HACK
		return dabs[brush_dab_index][(yc*64)+xc];
#else
		unsigned int dab_v[4];

		unsigned int x_f = (double)(xc_d - xc)*255;
		unsigned int y_f = (double)(yc_d - yc)*255;

		double top,bottom,mid;

		dab_v[0] = dabs[brush_dab_index][(yc*64)+xc];
		dab_v[1] = dabs[brush_dab_index][(yc*64)+xc+1];
		dab_v[2] = dabs[brush_dab_index][((yc+1)*64)+xc];
		dab_v[3] = dabs[brush_dab_index][((yc+1)*64)+xc+1];

		top = dab_v[0]*(255-x_f)+dab_v[1]*(x_f);
		bottom = dab_v[2]*(255-x_f)+dab_v[3]*(x_f);
		mid = (top*(255-y_f) + bottom*y_f)/(255*255);

#ifdef BRUSH_FANCY
		if(mid>128)
				if(mid<192)
				if( fastrand() < (RAND_MAX/2) )
						mid+=32;
		if(mid>1)
				if(mid<8)
				if( fastrand() < (RAND_MAX/2) )
						mid-=1;
#endif
		return (unsigned char)mid;
#endif
		//return (unsigned char)dab[(yc*64)+xc];
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

void brush_reset_random() {
	g_seed = 0;
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

		getMixedPaint(&current,brush_color_mix_mod);

		for( i=0; i<r2; ++i) {
				for( j=0; j<r2; ++j) {
						plotX += delta;
						//hax until i properly fix clipping
						if(( x + (j-r) ) < (signed int)ctxt->w)
						if(( y + (i-r) ) < (signed int)ctxt->h)
						if(( x + (j-r) ) > 0)
						if(( y + (i-r) ) > 0)
						{
								noise = 1-(((float)fastrand()/RAND_MAX)*brush_noise_mod);
								intensity = map_intensity(plotX,plotY,p);

#ifdef BRUSH_INTENSITY_TO_COLOR_MAPPING
								getMixedPaint(&current,(intensity*p)/255.0);
#endif
								if(coord>0 && coord<end && intensity>0) {
										buf = intensity*brush_alpha_mod*noise;
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
		if(ctxt == getDrawingContext() ) {
				invalidateDirty(x-r,y-r,x+r,y+r);
		}
}

void brushReset() {
}

/*
void brush_drawStrokeSegment_experimental(int x0, int y0, int x1, int y1,float p0,float p1, SDL_Surface* ctxt) {
		float pD = 1;

		double dX = (x1-x0);
		double dY = (y1-y0);
		double dP = (p1-p0);
 		double baseRadius = ((p0*pD)*brush_size_mod);
		double len = squareRoot(dX*dX+dY*dY) / 2;

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
 				radius = (int)((p0*pD)*brush_size_mod);
				plotSplat((int)x,(int)y,radius,p0, ctxt);
		}
		SDL_UnlockSurface(ctxt);
}*/

void brush_drawStrokeSegment(int x0, int y0, int x1, int y1,float p0,float p1, SDL_Surface* ctxt) {
		int origin_x = x0;
		int origin_y = y0;

	
		int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
		int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;

		int err = (dx>dy ? dx : -dy)/2, e2;

		int space_ctr = 0;

		float pD = 1;

		double p = ctxt == getDrawingContext() ? stylusFilter_getFilteredPressure() : p0;

		brush_modulate_values(p);
		SDL_LockSurface(ctxt);

		for(;;){
				// if we're drawing to the user's drawing context, we're going to use the fancy
				// filtered pressure, otherwise, we're rendering for some other reason, and
				// should use the supplied value.

				int radius = (int)(brush_size_mod)

#ifdef BRUSH_FANCY	
						+(int)(1.33*((float)fastrand() / RAND_MAX));
#else
						;
#endif

				int spacing = 
						(radius > 24 ) ?	
						2 + ( (radius*radius) / 700 ) :
						1;

				if (x0==x1 && y0==y1) break;
				e2 = err;
				if (e2 >-dx) { err -= dy; x0 += sx; }
				if (e2 < dy) { err += dx; y0 += sy; }

				if(space_ctr==0){
						int jtr_x = (((float)fastrand()) / RAND_MAX ) * (radius*brush_jitter_mod);
						int jtr_y = (((float)fastrand()) / RAND_MAX ) * (radius*brush_jitter_mod);
						plotSplat((x0+jtr_x),(y0+jtr_y),radius,p, ctxt);
				}
				space_ctr = (space_ctr+1) % spacing;
		}
		SDL_UnlockSurface(ctxt);
}
