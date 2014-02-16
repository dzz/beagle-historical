#include "surfaceCache.h"
#include "../document/animation.h"
#include "../compositor/compositor.h"
#include "../document/layers.h"
#include <stdio.h>
#include <SDL.h>
#include <string.h>

COMPOSITE_LAYER** surface_cache;

unsigned int surface_cache_size = SURFACE_CACHE_INITIAL_FRAMESTORAGE * MAX_LAYERS;
unsigned int current_frame_storage = SURFACE_CACHE_INITIAL_FRAMESTORAGE;

void allocateLayersForNewFrame(frame *fr) {
	unsigned int i = 0;
	for(i=0; i<MAX_LAYERS; ++i) {
		if(fr->layerKeyFrames[i]==1) {
				unsigned int coord = fr->idx + (i*current_frame_storage);
				if(surface_cache[coord] != 0) {
					printf("error! tried to initialize coord %d already allocated in framestash",coord);
				} else {
					surface_cache[coord]=allocateCompositeLayer(COMPOSITOR_DEFAULT_W,
																COMPOSITOR_DEFAULT_H);

					surface_cache[coord]->mode=layers[i].mode;

					//clear to background colour
					if(i==0) {
							SDL_FillRect(
										surface_cache[coord]->data,
										NULL,
										SDL_MapRGBA(surface_cache[coord]->data->format, 0x00,0x00,0x00,0x00)
										);
					}
				}
		}
	}
}

void destroySurfaceCache() {
	unsigned int i = 0;
	for( i = 0; i < surface_cache_size; ++i) {
		if(surface_cache[i]!=0) {
			destroyCompositeLayer((*surface_cache[i]));
			free(surface_cache[i]);
		}
	}
	free(surface_cache);
}

void initSurfaceCache() {
		surface_cache = (COMPOSITE_LAYER**)malloc(sizeof(COMPOSITE_LAYER*)*
						surface_cache_size);
		memset(surface_cache,0,surface_cache_size*sizeof(COMPOSITE_LAYER*));
}

COMPOSITE_LAYER findHeldFrame() {
		printf("call to findHeldFrame");
		exit(1);
}

COMPOSITE_LAYER* getCompositeLayerFromFrame(frame *fr, unsigned int layerIndex) {
		unsigned int coord = fr->idx + (layerIndex*current_frame_storage);
		if(surface_cache[coord]!=0) {
			return surface_cache[coord];
		} else {
			printf("tried to pull invalid layer");
			exit(1);
		}
}

SDL_Surface* compositeFrame(frame *fr, SDL_Rect r) {
		COMPOSITE comp;
		COMPOSITE_AREA area;
		COMPOSITE_LAYER* stack = (COMPOSITE_LAYER*)malloc(sizeof(COMPOSITE_LAYER)*numLayers);
		unsigned int i;

		area.x = (r.x > 0) ? r.x : 0;
		area.y = (r.y > 0) ? r.y : 0;
		area.w = r.w;
		area.h = r.h;

		for(i=0; i<numLayers;++i) {
				unsigned int coord = fr->idx + (i*current_frame_storage);
				if(surface_cache[coord]!=0) {
						stack[i] = (*surface_cache[coord]);
#ifdef COMPOSITE_MODE_PREFERENCE_LAYER
						stack[i].mode = layers[i].mode;
#endif
				} else {
						stack[i] = findHeldFrame();
				}
		}

		comp = compositeLayers(stack,numLayers,area);

		free(stack);
		return comp;
}

// this is the same as compositeFrame but injects a drawing context in place
// of the active layer
SDL_Surface* compositeFrameWithContext( DRAWING_CONTEXT context, frame *fr, SDL_Rect r) {

		COMPOSITE comp;
		COMPOSITE_AREA area;
		COMPOSITE_LAYER ctxt;
		COMPOSITE_LAYER* stack = (COMPOSITE_LAYER*)malloc(sizeof(COMPOSITE_LAYER)*numLayers);
		unsigned int i;

		area.x = (r.x > 0) ? r.x : 0;
		area.y = (r.y > 0) ? r.y : 0;
		area.w = r.w;
		area.h = r.h;

		for(i=0; i<numLayers;++i) {
				if( i == getActiveLayer() ) {
						ctxt.w = COMPOSITOR_DEFAULT_W;
						ctxt.h = COMPOSITOR_DEFAULT_H;
						ctxt.data = context;
						ctxt.mode = layers[getActiveLayer()].mode;
						stack[i] = ctxt;
						stack[i].mode = layers[i].mode; //inherit mode from layer manager
				} else {

						unsigned int coord = fr->idx + (i*current_frame_storage);
						if(surface_cache[coord]!=0) {
								stack[i] = (*surface_cache[coord]);
#ifdef COMPOSITE_MODE_PREFERENCE_LAYER
								stack[i].mode = layers[i].mode;
#endif
						} else {
								stack[i] = findHeldFrame();
						}
				}
		}

		comp = compositeLayers(stack,numLayers,area);

		free(stack);
		return comp;
}
