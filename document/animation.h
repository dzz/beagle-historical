#ifndef __ANIMATION__
#define __ANIMATION__

#include <SDL.H>
#include "../compositor/compositor.h"
#include "../document/layers.h"

typedef struct {
	int layerKeyFrames [ MAX_LAYERS ];
	int idx;
} frame;

void initFrames(void);
void dropFrames(void);

//controller methods
void anim_nav(SDL_Surface * drawingContext, int delta, int commit);
void anim_commit(void);

//debug methods
SDL_Surface * compositeWithContext(SDL_Surface * drawingContext, SDL_Rect r);
frame* getActiveFrame();

unsigned int frame_has_content(int idx);
unsigned int frame_has_layer_keyframe(int idx, int layer_idx);
frame* frame_find_held_frame(int idx, int layer);
#endif
