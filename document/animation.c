#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "animation.h"
#include "../system/ctt2.h"
#include "../system/surfaceCache.h"
#include "../drawing/drawingSurfaces.h"
#include "../compositor/compositor.h"

unsigned int baseSize = 1024;	//our initial stack

static unsigned int *frameMap;
static unsigned int animation_total_frames = 0;

frame **frameArr;
frame *activeFrame;

frame* find_implicit_create(int idx);

COMPOSITE_LAYER background;

frame *getActiveFrame() {
	return activeFrame;
}

void createFrame( unsigned int addr, unsigned int idx) {

		unsigned int i = 0;

		frameArr[addr] = malloc(sizeof(frame));
		frameArr[addr]->idx = idx;


		for(i=0; i< numLayers; ++i) {
			frameArr[addr]->layerKeyFrames[i]=0;
		}

		if(idx==0) {
			frameArr[addr]->layerKeyFrames[0] = 1;
			frameArr[addr]->layerKeyFrames[1] = 1;
		} //testing hack, background for first frame, rest don't get one
		else {
			frameArr[addr]->layerKeyFrames[0] = 0;
			frameArr[addr]->layerKeyFrames[1] = 1;
		}


		allocateLayersForNewFrame(frameArr[addr]);
		frameMap[idx] = addr;
}

void initFrames(void) {

		initSurfaceCache();

		frameArr=(frame **) malloc(baseSize*sizeof(frame*));
		memset(frameArr,0,baseSize*sizeof(frame*));

		{
			//an addr that fits inside 0...baseSize but won't have a value until
			//the buffer is full, needs to link to NULLs in frameArr
			//
			//later on when we are adjusting the baseSize this will need to be
			//recalculated based on the new baseSize
			unsigned int i;
			unsigned int invalid_addr = 1023;
			frameMap=(unsigned int*)malloc(baseSize*sizeof(unsigned int));

			for(i=0; i< baseSize; ++i) {
				fprintf(getLogfile(), "i = %d\n",i);
				frameMap[i] = invalid_addr;
			}
		}

		activeFrame = find_implicit_create(0);
}

/*find stored frame object for idx. If nothing found, return pointer
to new frame */

unsigned int findFreeAddr() {
	unsigned int i;

	for(i=0; i< baseSize; ++i) {
		if(frameArr[i]==0) {
			return i;
		}
	}
	exit(1); //you lose
}


frame* find_implicit_create(int idx) {
		int idx_origin_plus_one = idx+1;

		if( (idx_origin_plus_one) > animation_total_frames ) {
			animation_total_frames = idx_origin_plus_one;
		}

        if(frameArr[frameMap[idx]] == 0){
				createFrame(findFreeAddr(), idx);
				return frameArr[frameMap[idx]];
		}
		return frameArr[frameMap[idx]];
}

frame* frame_find_held_frame(int idx, int layer){
	int i = idx;
	do {
		frame *fr=frameArr[frameMap[i]];
		if(fr!=0){
				if(fr->layerKeyFrames[layer]==1) {
					return fr;
				}
		}
		i--;
	} while(i>=0);
	exit(1);
}


unsigned int frame_has_content(int idx){
	return frameArr[frameMap[idx]]!=0;
}

unsigned int frame_has_layer_keyframe(int idx, int layer) {
	if(!frame_has_content(idx))
		return 0;

	if(frameArr[frameMap[idx]]->layerKeyFrames[layer] == 1)
		return 1;

	return 0;
}

frame* find_left() {
	int i=0;
	int max=animation_total_frames;

	frame *found = 0;
	if(activeFrame->idx==0)
		return 0;
	do {
			int delta = 0;
			if( (frameArr[i]->idx < activeFrame->idx))
			{
							delta=(activeFrame->idx-frameArr[i]->idx );
							if(delta<max) {
								max=delta;
								found=frameArr[i];
							}
			}
			i++;
	} while(frameArr[i]!=0);
	return found;
}

void anim_nav(SDL_Surface * drawingContext, int delta, int commit) {

	COMPOSITE_LAYER* activeCompositeLayer = getCompositeLayerFromFrame( activeFrame, getActiveLayer(), COMP_RESOLVE_VIRTUAL	);

	if((activeFrame->idx==0) && (delta<0)) {
		return; 
	}

	if(commit == 1) {
		/*commit frame*/
		SDL_BlitSurface(drawingContext,NULL, activeCompositeLayer->data,NULL);
	}

	activeFrame = find_implicit_create(activeFrame->idx+delta);
	/*copy current frame to context*/
	activeCompositeLayer = getCompositeLayerFromFrame( activeFrame, getActiveLayer(), COMP_RESOLVE_VIRTUAL );
	SDL_BlitSurface(activeCompositeLayer->data,NULL, drawingContext,NULL);
}

void dropFrames(void) {
		free(frameArr);
		free(frameMap);
		destroyCompositeLayer(background);
		destroySurfaceCache();
}

void save(char *filename) {

}
