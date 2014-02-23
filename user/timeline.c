#include <SDL.h>
#include "timeline.h"

#include "../document/animation.h"

static const int num_layers = 2;
static const int layer_size = 60;
static const int length = 1920;

static SDL_Rect full_timeline;
static SDL_Rect layer_row;

static int frame_ticks = 24*4;

void initTimeline() {
	full_timeline.x=0;
	full_timeline.h=num_layers*layer_size;
	full_timeline.y=1080-full_timeline.h;
	full_timeline.w=1920;
}

void renderTimelineBackground(SDL_Surface* target) {
	layer_row.x = full_timeline.x;
	layer_row.y = full_timeline.y;
	layer_row.w = full_timeline.w;
	layer_row.h = layer_size;

	SDL_FillRect(target,&full_timeline,SDL_MapRGB(target->format, 0xFF,0xFF,0xFF ));

	{
			int i;
			for( i = 0; i< num_layers;++i) {
				if(i%2==1) {
						SDL_FillRect(target,&layer_row,SDL_MapRGB(target->format, 0xEE,0xEE,0xFF ));
				}
			}
	}
}

void renderTimelineFrameTicks(SDL_Surface* target) {
		float jmp = (float)full_timeline.w / (float)frame_ticks;
		frame aF = *getActiveFrame();

		int i;
		float h = 0;
		SDL_Rect marker;
		marker.h = layer_size;
		marker.w = jmp;
		for( i=0; i<frame_ticks;++i) {

				marker.x = (int)h;
				marker.y = (i%num_layers) * layer_size + full_timeline.y;
				SDL_FillRect(target,&marker,SDL_MapRGB(target->format, 0xAA,0xBB,0xCC ));

				if(frame_has_content(i)) {
					int j;
					for(j=0; j< num_layers;++j) {
						SDL_Rect data_marker = marker;
						data_marker.w=5;	
						data_marker.h=5;
						data_marker.y = full_timeline.y + (j*layer_size);
						if( frame_has_layer_keyframe(i,j)==1 ) {
							SDL_FillRect(target,
										&data_marker,
										SDL_MapRGB(target->format, 0x22,0x22,0x52 ));
						}
					}
				}
				h+=jmp;
		}

		SDL_Rect aF_marker;
		aF_marker.y = full_timeline.y;
		aF_marker.h = full_timeline.h;
		aF_marker.w = 5;
		aF_marker.x = jmp * aF.idx;
		SDL_FillRect(target,&aF_marker,SDL_MapRGB(target->format, 0x55,0x55,0x66 ));

}

void renderTimeline(SDL_Surface* target) {
	renderTimelineBackground(target);
	renderTimelineFrameTicks(target);
}

