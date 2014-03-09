#include "../system/ctt2.h"
#include "mapperEditorBank.h"
#include "brushEditor.h"

#include <SDL.h>

#define NUM_MAPPERS 5

#define NOT_EDITING_MAPPERS -1
#define MAPPER_NODE_MIN 0
#define MAPPER_NODE_MAX 1

static int editing_idx = NOT_EDITING_MAPPERS;
static int editing_node = NOT_EDITING_MAPPERS;

static mapping_function* mappers = 0;

static SDL_Rect computedDisplayRects[NUM_MAPPERS];
static SDL_Surface **mapperBmps;
static SDL_Surface *bgBmp;

void initMapperEditorBank(void) {
	if(mappers!=0) 
			return;

	mappers =(mapping_function*)malloc(sizeof(mapping_function)*NUM_MAPPERS);
	mapperBmps = (SDL_Surface**)malloc(sizeof(SDL_Surface*)*NUM_MAPPERS);
	{
		int i;
		for(i = 0; i < NUM_MAPPERS; ++i) {
				char fname[1024];
				mappers[i].min_x = 0.1;
				mappers[i].max_x = 0.9;
				mappers[i].min_y = 0.1;
				mappers[i].max_y = 0.9;
				mappers[i]._idx = i;

				if( (i==MAPPER_NOISE) ||
					(i==MAPPER_JITTER) ){
						mappers[i].min_x = 0.01;
						mappers[i].min_y = 0;
						mappers[i].max_x = 0.9;
						mappers[i].max_y = 0.01;
				}

				sprintf(fname,"ui_gen/mapper_%d.bmp",i);
				mapperBmps[i] = SDL_LoadBMP(fname);
		}
	}
	bgBmp = SDL_LoadBMP("ui_gen/mapper_bank_bg.bmp");
}


mapping_function* mapperbank_get_functions(void) {
	return mappers;
}

double mapperbank_compute_mapping(mapping_function* function,double input){
	if(input< function->min_x) return function->min_y;
	if(input> function->max_x) return function->max_y;
	
	input-=function->min_x;
	input/=(function->max_x - function->min_x);

	input*=(function->max_y-function->min_y);
	input+=function->min_y;

	return input;
}

unsigned int mapperbank_get_height() {
	return mapperBmps[0]->h;
}

double mapperbank_get_mapping(int idx, double input) {
		// shouldn't need this but the brush wants this
		// and it boots before we do
		if(mappers==0)
				initMapperEditorBank();

		return mapperbank_compute_mapping( &mappers[idx], input);
}

void destroyMapperEditorBank(void) {
		free(mappers);
		{
				int i;
				for(i=0; i< NUM_MAPPERS; ++i) {
					SDL_FreeSurface(mapperBmps[i]);
				}
		}
		free(mapperBmps);
		SDL_FreeSurface(bgBmp);
}

void renderMapperInRect(SDL_Surface *target, mapping_function *function, SDL_Rect *r) {
		unsigned int color_a = SDL_MapRGB( target->format, 0x00,0xFF,0x00);
		unsigned int color_b = SDL_MapRGB( target->format, 0x11,0xAA,0x11);

		int margin = 4;

		SDL_Rect inner_r;

		if(function->_idx == editing_idx){
				color_a = SDL_MapRGB( target->format, 0x11,0xFF,0x11 );	
				if(editing_node != NOT_EDITING_MAPPERS ) {
						color_b = SDL_MapRGB( target->format, 0x00,0xFF,0x00 );
				}
		}


		inner_r.w=r->w - (margin*2);
		inner_r.h=r->h - (margin*2);
		inner_r.x=r->x + margin;
		inner_r.y=r->y + margin;

		computedDisplayRects[function->_idx] = inner_r;

		{
			int i;
			int tesselation = 20;

			for(i=0; i< tesselation; ++i) {
				double input = (double) i / (double)tesselation;
				SDL_Rect plot;
				plot.w=2;
				plot.h=1;
				plot.x=inner_r.x + (int)(input*(double)inner_r.w);
				input = 1 - mapperbank_compute_mapping(function,input);
				plot.y=inner_r.y + (int)(input*(double)inner_r.h);

				SDL_FillRect(target,&plot,color_b);
			}
		}	

		{
			int hw = 2;
			int fw = hw*2;

			SDL_Rect handle_r;
			handle_r.x = inner_r.x + (function->min_x)*inner_r.w-hw;
			handle_r.y = inner_r.y + (1 - function->min_y)*inner_r.h-hw;
			handle_r.w = fw;
			handle_r.h = fw;

			function->_min_render_x = handle_r.x;
			function->_min_render_y = handle_r.y;

			SDL_FillRect(target,&handle_r,color_a);
			handle_r.x = inner_r.x + (function->max_x)*inner_r.w-hw;
			handle_r.y = inner_r.y + (1 - function->max_y)*inner_r.h-hw;
			SDL_FillRect(target,&handle_r,color_a);

			function->_max_render_x = handle_r.x;
			function->_max_render_y = handle_r.y;
		}
}

void renderMapperEditorBank(SDL_Surface* target, UI_AREA* area) {
		int i;
		int per_row = 5;
		SDL_BlitSurface(bgBmp,NULL,target,area);
		for(i=0; i<NUM_MAPPERS; ++i) {
			int column = i % per_row;
			int row = i / per_row;
			SDL_Rect r;

			r.x = column * mapperBmps[i]->w + area->x;
			r.y = row * mapperBmps[i]->h + area->y;
			r.w = mapperBmps[i]->w;
			r.h = mapperBmps[i]->h;
			SDL_BlitSurface(mapperBmps[i],NULL,target,&r);
			renderMapperInRect(target,&mappers[i],&r);
		}
}


unsigned int pointInMapper(int x, int y, int i,UI_AREA *area) {
		if( ( x > computedDisplayRects[i].x ) && ( y > computedDisplayRects[i].y ) &&
			( x < computedDisplayRects[i].x + computedDisplayRects[i].w ) && (y < computedDisplayRects[i].y + computedDisplayRects[i].h ) ) {
				return 1;
		}
		return 0;
}

void mapperbank_mousedown(int x, int y, UI_AREA *area){
		const int md_threshold = 400;
		int i;

		// 
		x = client_get_screen_mousex();
		y = client_get_screen_mousey();

		for(i=0; i< NUM_MAPPERS; ++i) {
				if(pointInMapper(x,y,i,area) == 1) {
						struct { unsigned int x, y; }* nodes = &mappers[i]._min_render_x;
						int node_idx;
						editing_idx = i;
						for(node_idx=0; node_idx <2; ++node_idx) {
								int dx = x - nodes[node_idx].x;
								int dy = y - nodes[node_idx].y;
								int md = dx*dx+dy*dy;
								if( md < md_threshold ) {
										editing_node = node_idx; // this will correspond to MAPPER_NODE_MIN or MAPPER_NODE_MAX
								}
						}
				}
		}
}

void mapperbank_mouseup(int x, int y, UI_AREA *area){
		editing_idx = NOT_EDITING_MAPPERS;
		editing_node = NOT_EDITING_MAPPERS;
		brusheditor_redraw_stroke();
}

void mapperbank_mousemotion(int x, int y, UI_AREA *area){

		x = client_get_screen_mousex();
		y = client_get_screen_mousey();

		if( editing_idx != NOT_EDITING_MAPPERS ) {

				if(pointInMapper(x,y,editing_idx,area) == 0) {
					editing_idx = NOT_EDITING_MAPPERS;
					editing_node = NOT_EDITING_MAPPERS;
					return;
				}

				if ( editing_node != NOT_EDITING_MAPPERS ) {
						double* node_points = &mappers[editing_idx];
						double t_x = (double)(x - computedDisplayRects[editing_idx].x)/ (double)computedDisplayRects[editing_idx].w;
						double t_y = (double)(y - computedDisplayRects[editing_idx].y)/ (double)computedDisplayRects[editing_idx].h;

						t_y = 1 - t_y;

						if( editing_node == MAPPER_NODE_MIN ) {
								node_points[2] = t_y;

								if(t_x < node_points[1] ){
										node_points[0] = t_x;
								}
						}

						if( editing_node == MAPPER_NODE_MAX ) {
								node_points[3] = t_y;

								if(t_x > node_points[0] )
										node_points[1] = t_x;
						}
				}
		}
}

void mapperbank_mouseleave(){
		editing_idx = NOT_EDITING_MAPPERS;
		editing_node = NOT_EDITING_MAPPERS;
}
