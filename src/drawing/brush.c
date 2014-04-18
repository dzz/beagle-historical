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

#include "node_mapper.h"

#include "shader_brush.h"
#include "brush.h"

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

static double brush_rot;

static stylusState stroke_origin;

unsigned char (*active_mixing_function)(unsigned char,unsigned char,unsigned char);


void brush_modulate_values() {
        //eventually, what should happen here is to keep track of 2 or more
        //states so we can smoothly interpolate when we tesselate

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
        brush_rot = 
                brush_controller->outputs[BRUSH_CHANNEL_ROT];

		if(brush_r>1) brush_r = 1;
		if(brush_g>1) brush_g = 1;
		if(brush_b>1) brush_b = 1;
}

#define MAX_DABS 32
SDL_Surface* dab_bmps[MAX_DABS];

SDL_Surface* brush_get_active_dab_bmp() {
	if(dab_bmps[brush_dab_index]!=0)
			return dab_bmps[brush_dab_index];
	else
			//hax
			return dab_bmps[0];
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

    hw_import_dab_texture(dab_bmps[brush_dab_index]);

}


void initBrush() {
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
			brush_loaded_dabs++;
	}
}

void dropBrush() {
		int i;
		for( i=0; i< MAX_DABS; ++i) {
			if(dab_bmps[i]!=0) {
				SDL_FreeSurface(dab_bmps[i]);
			}
		}
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

static void mix_rgb_by_float(uint_rgba_map *pix, float p, cp_color prim, cp_color secon) {
		pix->rgba.r = (unsigned char)((float)prim.r * p + (float)secon.r * (1-p));
		pix->rgba.g = (unsigned char)((float)prim.g * p + (float)secon.g * (1-p));
		pix->rgba.b = (unsigned char)((float)prim.b * p + (float)secon.b * (1-p));
}


void brush_begin_stroke( stylusState a ) {
		stroke_origin = a;
        brush_modulate_values();
}

void brush_render_stylus_stroke(stylusState a, stylusState b) {
	brush_tesselate_stroke(a.x,a.y,b.x,b.y,
					(float)a.pressure,(float)b.pressure, 
					a.timestamp,b.timestamp);
}

void brush_end_stroke() {
    //hw_brush_commit_brush_stroke(); 
    return;
}

void brush_tesselate_stroke(int x0, int y0, int x1, int y1,float p0,float p1, unsigned int t0, unsigned int t1) {
       
		int origin_x = x0;
		int origin_y = y0;
	
		int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
		int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;

		int err = (dx>dy ? dx : -dy)/2, e2;

		float	radius = (float)(brush_size_mod);

        for(;;){
            int jtr_x = (((float)fastrand()) / RAND_MAX ) * (radius*brush_jitter_mod);
            int jtr_y = (((float)fastrand()) / RAND_MAX ) * (radius*brush_jitter_mod);

		    brush_modulate_values();

            radius = (float)brush_size_mod;

            if (x0==x1 && y0==y1) break;
            e2 = err;
            if (e2 >-dx) { err -= dy; x0 += sx; }
            if (e2 < dy) { err += dx; y0 += sy; }


            hw_brush_dab((float)((x0+jtr_x)),
                    (float)((y0+jtr_y)),
                    (float)radius, 
                    (float)brush_r,
                    (float)brush_g,
                    (float)brush_b,
                    (float)brush_alpha_mod,
                    (float)brush_noise_mod,
                    (float)brush_rot);
        }
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


static int g_seed = 0;

__inline int fastrand() { 
  g_seed = (214013*g_seed+2531011); 
  return (g_seed>>16)&0x7FFF; 
} 

void brush_reset_random() {
	g_seed = 0;
}
