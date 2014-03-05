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

#define COMMIT_DRAWING_CONTEXT 1
#define DO_NOT_COMMIT_DRAWING_CONTEXT 0

void animation_cursor_move(SDL_Surface * drawingContext, int delta, int commit);
void animation_insert_keyframe_at_cursor();
void animation_delete_keyframe_at_cursor();

void anim_commit(void);

frame* getActiveFrame();

unsigned int frame_has_content(int idx);
unsigned int frame_has_layer_keyframe(int idx, int layer_idx);
frame* frame_find_held_frame(int idx, int layer);

void animation_save();
void animation_load();

#endif
