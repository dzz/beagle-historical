#define MIN(a,b) (((a)<(b))?(a):(b))
#define CLAMP(a,b) (unsigned char)(((a)>(b))?(a):(b))

#include <stdio.h>
#include <malloc.h>

#include "compositor.h"

#include "../system/ctt2.h"
#include "../drawing/drawingSurfaces.h"
#include "../colors/colors.h"


const int CMP_ADD=0;
const int CMP_SUB=1;
const int CMP_SRC=2;
const int CMP_ZERO=3;
const int CMP_ALPHA_OVER=4;


COMPOSITE createFlatCompositeTarget(int w, int h) {
	return (COMPOSITE)createDrawingSurface(w,h);
}

COMPOSITE_LAYER createCompositeLayer(int w, int h) {
	COMPOSITE_LAYER layer;
	layer.w = w;
	layer.h = h;
	layer.data = (SDL_Surface *)createFlatCompositeTarget(w, h);
	layer.mode = CMP_ADD;
	layer.x = 0;
	layer.y = 0;
	return layer;
}

COMPOSITE_LAYER *allocateCompositeLayer(int w, int h) {
	COMPOSITE_LAYER layer = createCompositeLayer(w,h);
	COMPOSITE_LAYER *heaped = malloc(sizeof(COMPOSITE_LAYER));
	heaped->w = layer.w;
	heaped->h = layer.h;
	heaped->data = layer.data;
	heaped->x = layer.x;
	heaped->y = layer.y;
	return heaped;
}

void destroyCompositeLayer(COMPOSITE_LAYER layer) {
	SDL_FreeSurface(layer.data);
}

void deleteComposite(COMPOSITE composite) {
	SDL_FreeSurface((SDL_Surface *)composite);
}

void prepareLayer(COMPOSITE_LAYER layer) {
	SDL_LockSurface(layer.data);
}

void releaseLayer(COMPOSITE_LAYER layer) {
	SDL_UnlockSurface(layer.data);
}

void prepareComposite(COMPOSITE target) {
	SDL_LockSurface(target);
}

void releaseComposite(COMPOSITE target) {
	SDL_UnlockSurface(target);
}

unsigned int * getCompositeData(COMPOSITE target) {
	return (unsigned int*)target->pixels;
}

unsigned int * getLayerData(COMPOSITE_LAYER target) {
	return (unsigned int*)target.data->pixels;
}

unsigned char clamp_add(unsigned char l,unsigned char r,unsigned char src_a) {
	int acc = l + r;
	if(acc>255) acc = 255;
	return (unsigned char)acc;
}

unsigned char clamp_sub(unsigned char l,unsigned char r,unsigned char src_a) {
	int acc = r - l;
	if(acc>255) acc = 255;
	if(acc<0) acc = 0;
	return (unsigned char)acc;
}

unsigned char clamp_src(unsigned char l,unsigned char r,unsigned char src_a) {
	return l;
}

unsigned char comp_zero(unsigned char l, unsigned char r,unsigned char src_a) {
	return 0;
}

unsigned char comp_alpha_over_ref(unsigned char l, unsigned char r,unsigned char src_a) {
		float blend = (float)src_a/255;
		float computed = ((float)l * blend)+((float)r * (1.0f-blend));
		return (unsigned char)(computed);
}

unsigned char comp_alpha_over(unsigned char l, unsigned char r,unsigned char src_a) {
		//float blend = (float)src_a;
		unsigned int computed = (l * src_a)+(r * (255-src_a));
		return (unsigned char)(computed/255);
}

unsigned char (*functab[5])(unsigned char,unsigned char,unsigned char);
void initCompositor(void) {
	functab[CMP_ADD] = &clamp_add;
	functab[CMP_SUB] = &clamp_sub;
	functab[CMP_SRC] = &clamp_src;
	functab[CMP_ZERO] = &comp_zero;
	functab[CMP_ALPHA_OVER] = &comp_alpha_over;
}

COMPOSITE compositeLayers(COMPOSITE_LAYER *stack, int len, COMPOSITE_AREA area){
	COMPOSITE flat = createFlatCompositeTarget(area.w,area.h);	
	int i;
	for(i=0; i<len;++i) {
		SDL_SetSurfaceBlendMode(stack[i].data, SDL_BLENDMODE_BLEND);
		SDL_BlitSurface(stack[i].data, (SDL_Rect*)&area, flat, NULL);
		drawing_surface_restore_default_blending( stack[i].data);
	}
	return flat;
}

COMPOSITE compositeLayers_full_effects(COMPOSITE_LAYER *stack, int len, COMPOSITE_AREA area) {
	COMPOSITE flat=createFlatCompositeTarget(area.w,area.h);
	int start = area.y * stack[0].w + area.x;
	int end = (area.y+area.h) * stack[0].w;
	int read = start;
	int put = 0;
	int jump = stack[0].w - (area.w) + 1;
	int scan = 0;
	int scanline = 0;
	int l;
	unsigned int * dest = getCompositeData(flat);
	unsigned int * source;
	unsigned char (*func)(unsigned char, unsigned char, unsigned char);
	uint_rgba_map src;
	uint_rgba_map dst;
	uint_rgba_map acc;


	if (start<0) start = 0;
	if (end>=(stack[0].w*stack[0].h)-1) end = (stack[0].w*stack[0].h)-1;

	for(l = 0; l < len; ++l) {
		prepareLayer(stack[0]);
	}

	prepareComposite(flat);
	do{
			for(l = 0; l < len; ++l) {
					func = functab[stack[l].mode];
					source = getLayerData(stack[l]);
					{
							src.packed = source[read];
							dst.packed = dest[put];
							acc.rgba.r = (*func)(src.rgba.r,dst.rgba.r,src.rgba.a);
							acc.rgba.g = (*func)(src.rgba.g,dst.rgba.g,src.rgba.a);
							acc.rgba.b = (*func)(src.rgba.b,dst.rgba.b,src.rgba.a);
							acc.rgba.a = 255;
							dest[put] = acc.packed;
					}
			}
			scan ++;
			if (scan >= area.w) {
					scan = 0;
					read+=jump;
					scanline++;
			} else {
					read++;
			}
			put ++;

	} while (read <end);
	releaseComposite(flat);
	for(l = 0; l < len; ++l) {
			releaseLayer(stack[0]);
	}
	return flat;
}

