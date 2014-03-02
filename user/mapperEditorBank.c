#include "mapperEditorBank.h"
#include <SDL.h>

#define NUM_MAPPERS 5
#define MAPPER_BANK_HEIGHT (128*3)


static mapping_function* mappers;
static SDL_Rect computedDisplayRects[NUM_MAPPERS];
static SDL_Surface **mapperBmps;
static SDL_Surface *bgBmp;

void initMapperEditorBank(void) {
	mappers =(mapping_function*)malloc(sizeof(mapping_function)*NUM_MAPPERS);
	mapperBmps = (SDL_Surface**)malloc(sizeof(SDL_Surface*)*NUM_MAPPERS);
	{
		int i;
		for(i = 0; i < NUM_MAPPERS; ++i) {
				char fname[1024];
				mappers[i].min_x = 0;
				mappers[i].max_x = 1;
				mappers[i].min_y = 0;
				mappers[i].max_y = 1;
				mappers[i]._idx = i;

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
		int margin = 12;

		SDL_Rect inner_r;

		inner_r.w=r->w - (margin*2);
		inner_r.h=r->h - (margin*2);
		inner_r.x=r->x + margin;
		inner_r.y=r->y + margin;

		computedDisplayRects[function->_idx] = inner_r;

		{
			int i;
			int tesselation = 25;

			for(i=0; i< tesselation; ++i) {
				double input = (double) i / (double)tesselation;
				SDL_Rect plot;
				plot.w=2;
				plot.h=2;
				plot.x=inner_r.x + (int)(input*(double)inner_r.w);
				input = 1 - mapperbank_compute_mapping(function,input);
				plot.y=inner_r.y + (int)(input*(double)inner_r.h);

				SDL_FillRect(target,&plot,color_b);
			}
		}	

		{
			SDL_Rect handle_r;
			handle_r.x = inner_r.x + (function->min_x)*inner_r.w;
			handle_r.y = inner_r.y + (1 - function->min_y)*inner_r.h;
			handle_r.w = 4;
			handle_r.h = 4;
			SDL_FillRect(target,&handle_r,color_a);
			handle_r.x = inner_r.x + (function->max_x)*inner_r.w;
			handle_r.y = inner_r.y + (1 - function->max_y)*inner_r.h;
			SDL_FillRect(target,&handle_r,color_a);
		}
}

void renderMapperEditorBank(SDL_Surface* target, UI_AREA* area) {
		int i;
		int per_row = 4;
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
