#include "animation.h"
#include "../system/ctt2.h"
#include "../system/surfaceCache.h"
#include "../drawing/drawingSurfaces.h"
#include "../compositor/compositor.h"

#include <stdio.h>
#include <malloc.h>
#include <string.h>

unsigned int baseSize = 1024;	//our initial stack

static unsigned int *frameMap;
frame **frameArr;
frame *activeFrame;

frame* find(int idx);

COMPOSITE_LAYER background;

frame *getActiveFrame() {
	return activeFrame;
}

void createFrame( unsigned int addr, unsigned int idx) {

		unsigned int i = 0;

		frameArr[addr] = malloc(sizeof(frame));
		frameArr[addr]->idx = idx;
		frameArr[addr]->drawing = createDrawingSurface(1920,1080);


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
		printf(" allocd fr:%d\n", frameArr[frameMap[idx]]);
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

		activeFrame = find(0);
}

/*find stored frame object for idx. If nothing found, return pointer
to new frame

should opto with sorting

caching eventually needed

*/

unsigned int findFreeAddr() {
	unsigned int i;

	for(i=0; i< baseSize; ++i) {
		fprintf(getLogfile(), "i = %d frameptr %d \n",i,frameArr[i]);
		if(frameArr[i]==0) {
			return i;
		}
	}
	exit(1); //you lose
}


frame* find(int idx) {
		fprintf(getLogfile(), "idx = %d\n",idx);
		fprintf(getLogfile(), "frame map @ idx = %d\n",frameMap[idx]);
		fprintf(getLogfile(), "frame arr in map = %d\n",frameArr[frameMap[idx]]);

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
	printf("developer loses this round");
	exit(1);
}


unsigned int frame_has_content(int idx){
	return frameArr[frameMap[idx]]!=0;
}

frame* find_left() {
	int i=0;
	int max=999999999; //this is dumb!

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

	COMPOSITE_LAYER* activeCompositeLayer = getCompositeLayerFromFrame( activeFrame, getActiveLayer() );

	if((activeFrame->idx==0) && (delta<0)) {
		return; //sorry bubsy
	}

	if(commit == 1) {
		/*commit frame*/
		SDL_BlitSurface(drawingContext,NULL, activeCompositeLayer->data,NULL);
	}

	activeFrame = find(activeFrame->idx+delta);
	/*copy current frame to context*/
	activeCompositeLayer = getCompositeLayerFromFrame( activeFrame, getActiveLayer() );
	SDL_BlitSurface(activeCompositeLayer->data,NULL, drawingContext,NULL);
}

void dropFrames(void) {
		int i;
		for(i=0; i<baseSize;++i) {
			if(frameArr[i]!=0) {
				SDL_FreeSurface(frameArr[i]->drawing);
			}
		}
		free(frameArr);
		free(frameMap);
		destroyCompositeLayer(background);
		destroySurfaceCache();
}


