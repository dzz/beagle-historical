#ifndef __SURFACE_CACHE__
#define __SURFACE_CACHE__

#define COMPOSITE_MODE_PREFERENCE_LAYER
#define SURFACE_CACHE_INITIAL_FRAMESTORAGE 2048

#include "../document/animation.h"
#include "../compositor/compositor.h"
#include "../drawing/drawingSurfaces.h"
#include <SDL.h>

void allocateLayersForNewFrame(frame *fr);
void destroySurfaceCache(void);
void initSurfaceCache(void);
SDL_Surface* compositeFrame(frame *fr, SDL_Rect r);
SDL_Surface* compositeFrameWithContext( DRAWING_CONTEXT context, frame *fr, SDL_Rect r);
COMPOSITE_LAYER* getCompositeLayerFromFrame(frame *fr, unsigned int layerIndex);

#endif

/****  reference for pointers
 
struct foo
{
  int x;
  float y;
};

struct foo var;
struct foo* pvar;

var.x = 5;
(&var)->y = 14.3;
pvar->y = 22.4;
(*pvar).x = 6;


 ****/
