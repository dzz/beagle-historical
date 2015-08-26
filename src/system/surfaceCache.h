#ifndef __SURFACE_CACHE__
#define __SURFACE_CACHE__

#define COMPOSITE_MODE_PREFERENCE_LAYER
#define SURFACE_CACHE_INITIAL_FRAMESTORAGE 2048

#include <SDL.h>


void destroySurfaceCache(void);
void initSurfaceCache(void);

#endif
