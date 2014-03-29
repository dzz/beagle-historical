//   dopey - paint synthesizer
//
//  	#define BRUSH_FANCY			- secret noise and dithering
//


#define BRUSH_FANCY

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#include <SDL.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "../system/ctt2.h"
#include "../system/dirty.h"

#include "../colors/colors.h"
#include "../compositor/compositor.h"

#include "../user/stylus.h"

#include "../user/editors/toolbar.h"
#include "../user/editors/colorPicker.h"
#include "../user/editors/brushEditor.h"
#include "../user/editors/mapperEditorBank.h"

#include "drawingSurfaces.h"
#include "drawingContext.h"

#include "node_mapper.h"

#include "hw_brush_context.h"
#include "brush.h"

static SDL_Surface* brush_drawing_context;
static SDL_Surface* smudge_buffer;

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
static int brush_loaded_dabs = 0;
static int brush_smudge = 0;

static double brush_r;
static double brush_g;
static double brush_b;

static stylusState stroke_origin;

unsigned char (*active_mixing_function)(unsigned char,unsigned char,unsigned char);

unsigned char mix_char(unsigned char l, unsigned char r, unsigned char idx);
unsigned char bright_char(unsigned char l, unsigned char r, unsigned char idx);
unsigned char dark_char(unsigned char l, unsigned char r, unsigned char idx);

double test_modulate(unsigned int time_ms) {
	return cos( ((double)time_ms/1000)*25*3.14 );
	//return 1;
}

void brush_modulate_values(double pressure, unsigned int time_ms) {
		mapper_node* brush_controller = nodemapper_get_brush_controller();

		brush_size_mod = 
				brush_size_base * 
				brush_controller->outputs[BRUSH_CHANNEL_SIZE];
		brush_alpha_mod = 
				brush_controller->outputs[BRUSH_CHANNEL_ALPHA];

		brush_jitter_mod = 
				brush_controller->outputs[BRUSH_CHANNEL_JITTER];

		brush_noise_mod = 
				brush_controller->outputs[BRUSH_CHANNEL_NOISE];
		brush_r = 
				brush_controller->outputs[BRUSH_CHANNEL_R];
		brush_g = 
				brush_controller->outputs[BRUSH_CHANNEL_G];
		brush_b = 
				brush_controller->outputs[BRUSH_CHANNEL_B];

		if(brush_r>1) brush_r = 1;
		if(brush_g>1) brush_g = 1;
		if(brush_b>1) brush_b = 1;
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


	brush_erase =  (get_selected_tool() == TOOL_ERASE ) ? 1 : 0;
	brush_smudge = (get_selected_tool() == TOOL_SMUDGE) ? 1 : 0;

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

	initNodeMapper();
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
			brush_drawing_context = context;

			SDL_LockSurface(dabBmp);
			for(i=0; i<64*64;++i) {
					unsigned char* dabData = (unsigned char*)dabBmp->pixels;
					dabs[idx][i] = (double)255-dabData[i];
			}

			SDL_UnlockSurface(dabBmp);
			brush_loaded_dabs++;
	}

	smudge_buffer = createDrawingSurface( context->w, context->h );
}

void destroyBrush() {
		int i;
		for( i=0; i< MAX_DABS; ++i) {
			if(dab_bmps[i]!=0) {
				SDL_FreeSurface(dab_bmps[i]);
			}
		}

		SDL_FreeSurface(smudge_buffer);
		dropNodeMapper();
}


static uint_rgba_map mixed;

unsigned int* erase(uint_rgba_map src,uint_rgba_map dst) {

	mixed.rgba.r = dst.rgba.r;
	mixed.rgba.g = dst.rgba.g;
	mixed.rgba.b = dst.rgba.b;

	if(dst.rgba.a<src.rgba.a) mixed.rgba.a = 0; else mixed.rgba.a=dst.rgba.a-src.rgba.a;
	return &mixed.packed;
}

unsigned int* mix(uint_rgba_map src, uint_rgba_map dst) {
		unsigned int alpha;

		if(dst.rgba.a == 0) {
				dst.rgba.r = mixed.rgba.r;
				dst.rgba.g = mixed.rgba.g;
				dst.rgba.b = mixed.rgba.b;
		}

		mixed.rgba.r = (*active_mixing_function)(src.rgba.r,dst.rgba.r,src.rgba.a);
		mixed.rgba.g = (*active_mixing_function)(src.rgba.g,dst.rgba.g,src.rgba.a);
		mixed.rgba.b = (*active_mixing_function)(src.rgba.b,dst.rgba.b,src.rgba.a);

		{
				alpha = src.rgba.a+dst.rgba.a;
				if(alpha>255)
						alpha = 255;

		}
		mixed.rgba.a = (unsigned char)alpha;
		return &mixed.packed;

}

__inline float map_intensity(float x,float y,float p) {
		float  xc_d = (x*32)+32;
		float  yc_d = (y*32)+32;

		int xc=(int)xc_d;
		int yc=(int)yc_d;

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
		// dopey dither
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
}

static void mix_rgb_by_float(uint_rgba_map *pix, float p, cp_color prim, cp_color secon) {
		pix->rgba.r = (unsigned char)((float)prim.r * p + (float)secon.r * (1-p));
		pix->rgba.g = (unsigned char)((float)prim.g * p + (float)secon.g * (1-p));
		pix->rgba.b = (unsigned char)((float)prim.b * p + (float)secon.b * (1-p));
}


__inline void plot_dab(int x, int y, int r, float p, SDL_Surface* ctxt) {
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
		uint_rgba_map dest;
		uint_rgba_map current;
		unsigned int* pixels = (unsigned int*)ctxt->pixels;
		unsigned int* smudge_pixels = (unsigned int*)smudge_buffer->pixels;
		int clipped_x = ctxt->w -(x+r);
		const int ctxt_end = ctxt->w*ctxt->h;
		float noise = 1;
		unsigned int* (*mixer)(uint_rgba_map,uint_rgba_map);
		float buf;


		clipped_x = clipped_x < 0 ? r2 + clipped_x : r2;


		if(brush_erase == 1) {
				mixer = &erase;
		} 
		else {
				mixer = &mix;
		}

		if( brush_smudge != 1) {
				
				current.rgba.r = (unsigned char)(brush_r *255.0);
				current.rgba.g = (unsigned char)(brush_g *255.0);
				current.rgba.b = (unsigned char)(brush_b *255.0);
			//mix_rgb_by_float(&current,brush_color_mix_mod, getPrimaryColor(), getSecondaryColor());
		} else {
			current = sample_surface( ctxt, x,y );
		}
		for( i=0; i<r2; ++i) {
				for( j=0; j<r2; ++j) {

						plotX += delta;
						//hax until i properly fix clipping
						if(( x + (j-r) ) < (signed int)ctxt->w)
						if(( y + (i-r) ) < (signed int)ctxt->h)
						if(( x + (j-r) ) > 0)
						if(( y + (i-r) ) > 0)
						{
								if( brush_smudge == 1 ) {
										if( ctxt == brush_drawing_context ) {

											int smudge_coord = 
													  (i-r) + 
													  ( (j-r) * smudge_buffer->w) +
													  (stroke_origin.x) +
													  (stroke_origin.y * smudge_buffer->w);

											if(smudge_coord>0 && smudge_coord < ctxt_end ) {
												current.packed = smudge_pixels[(unsigned int)smudge_coord];
											}
										}
								}

								noise = 1-(((float)fastrand()/RAND_MAX)*brush_noise_mod);
								intensity = map_intensity(plotX,plotY,p);

								if(coord>0 && coord<ctxt_end && intensity>0) {
										buf = intensity*brush_alpha_mod*noise;
										v = (unsigned char)(buf);
										dest.packed =pixels[coord];
										current.rgba.a = v;
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


void brush_begin_stroke( stylusState a ) {
		stroke_origin = a;

		if( brush_smudge == 1 ) {
			SDL_BlitSurface( getDrawingContext() , NULL, smudge_buffer, NULL);
			SDL_LockSurface( smudge_buffer );
		} 
}

void brush_render_stylus_stroke(stylusState a, stylusState b) {
	brush_tesselate_stroke(a.x,a.y,b.x,b.y,
					(float)a.pressure,(float)b.pressure, 
					a.timestamp,b.timestamp,
					brush_drawing_context);
}

void brush_end_stroke() {
		SDL_UnlockSurface( smudge_buffer );
}

void brush_tesselate_stroke(int x0, int y0, int x1, int y1,float p0,float p1, unsigned int t0, unsigned int t1, SDL_Surface* ctxt) {
		int origin_x = x0;
		int origin_y = y0;
	
		int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
		int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;

		int err = (dx>dy ? dx : -dy)/2, e2;

		int space_ctr = 0;

		float pD = 1;

		// if we're drawing to the user's drawing context, we're going to use the fancy
		// filtered pressure, otherwise, we're rendering for some other reason, and
		// should use the supplied value.
		//
		//this is not ideal coupling

		double p = ctxt == getDrawingContext() ? stylusFilter_getFilteredPressure() : p0;

		int	radius = (int)(brush_size_mod);
		int spacing = 1;

		brush_modulate_values(p, t0);
		SDL_LockSurface(ctxt);
		
		//set_smudge_sample(ctxt,x0,y0,x1,y1);

		for(;;){

				radius = (int)(brush_size_mod)
#ifdef BRUSH_FANCY	
						+(int)(1.27*((float)fastrand() / RAND_MAX)) // noise 
#endif
						;


				if (x0==x1 && y0==y1) break;
				e2 = err;
				if (e2 >-dx) { err -= dy; x0 += sx; }
				if (e2 < dy) { err += dx; y0 += sy; }

				if(space_ctr==0){
						int jtr_x = (((float)fastrand()) / RAND_MAX ) * (radius*brush_jitter_mod);
						int jtr_y = (((float)fastrand()) / RAND_MAX ) * (radius*brush_jitter_mod);

						hw_brush_dab((float)((x0+jtr_x)),
                                (float)((y0+jtr_y)),
                                (float)radius, 
                                (float)brush_r,
                                (float)brush_g,
                                (float)brush_b,
                                (float)brush_alpha_mod);
				}
				space_ctr = (space_ctr+1) % spacing;
		}
		SDL_UnlockSurface(ctxt);
}

__inline float cheap_sqrt(float x)
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
		float d = 1 - cheap_sqrt((x*x)+(y*y));
		if( d<0) return 0;
		d *= 1 / brush_power;
		if(d>1) d = 1;
		return d*255;
}
*/

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

static int g_seed = 0;

__inline int fastrand() { 
  g_seed = (214013*g_seed+2531011); 
  return (g_seed>>16)&0x7FFF; 
} 

void brush_reset_random() {
	g_seed = 0;
}
