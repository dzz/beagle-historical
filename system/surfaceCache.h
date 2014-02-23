#ifndef __SURFACE_CACHE__
#define __SURFACE_CACHE__

#define COMPOSITE_MODE_PREFERENCE_LAYER
#define SURFACE_CACHE_INITIAL_FRAMESTORAGE 2048

#include "../document/animation.h"
#include "../compositor/compositor.h"
#include "../drawing/drawingSurfaces.h"
#include <SDL.h>


void allocateLayersForNewFrame(frame *fr);
void replaceLayerForExistingFrame(frame *fr, int layer, SDL_Surface *newLayer);
void destroySurfaceCache(void);
void initSurfaceCache(void);
SDL_Surface* compositeFrame(frame *fr, SDL_Rect r);
SDL_Surface* compositeFrameWithContext( DRAWING_CONTEXT context, frame *fr, SDL_Rect r,unsigned char field_mode);


// COMP_RESOLVE modes:
//      VIRTUAL - if no keyframe on the layer, will resolve to previous keyframe
//      ACTUAL - returns 0 if a nonexistant keyframe is queries for

#define COMP_RESOLVE_VIRTUAL 1
#define COMP_RESOLVE_ACTUAL 0

COMPOSITE_LAYER* getCompositeLayerFromFrame(frame *fr, unsigned int layerIndex, unsigned int resolveVirtual);

#endif
