#ifndef __GFX_BLEND_CONTROL__
#define __GFX_BLEND_CONTROL__

#define BLENDMODE_OVER 0
#define BLENDMODE_BRUSH_COMPOSITE 1
#define BLENDMODE_DAB_RENDERING 2

#define BLENDMODE_BLACKHOLE 600

#define BLENDMODE_TESTING 1

#define BLENDMODE_ADD       5000
#define BLENDMODE_DARKEN    6000
#define BLENDMODE_DARKEN2   6001


void blend_enter(unsigned int mode);
void blend_exit();
void manual_blend_enter(unsigned int mode);
void manual_blend_exit();

#endif 

