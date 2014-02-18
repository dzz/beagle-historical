#include "animation.h"
#include "../drawing/drawingSurfaces.h"
#include <stdio.h>
#include "../compositor/compositor.h"
#include "../system/surfaceCache.h"
#include <malloc.h>

int baseSize = 1024;	//our initial stack

frame **frameArr;
frame *activeFrame;

frame* find(int idx);

COMPOSITE_LAYER background;

frame *getActiveFrame() {
	return activeFrame;
}

void createFrame( unsigned int addr, int idx) {
		unsigned int i = 0;

		frameArr[addr] = malloc(sizeof(frame));
		frameArr[addr]->idx = idx;
		frameArr[addr]->drawing = createDrawingSurface(1920,1080);


		for(i=0; i< numLayers; ++i) {
			frameArr[addr]->layerKeyFrames[i]=0;
		}

		frameArr[addr]->layerKeyFrames[0] = 1;
		frameArr[addr]->layerKeyFrames[1] = 1;

		allocateLayersForNewFrame(frameArr[addr]);
}

void initFrames(void) {
		SDL_Surface * bgImage = SDL_LoadBMP("c:\\res\\test.bmp");
		background = createCompositeLayer(1920,1080);

		initSurfaceCache();

		SDL_BlitSurface(bgImage,NULL,background.data,NULL);
		SDL_FreeSurface(bgImage);

		//SDL_FillRect( background.data, NULL, SDL_MapRGB( background.data->format, 0x00, 0xFF, 0x00 ) );
		frameArr=(frame **) malloc(baseSize*sizeof(frame*));
		memset(frameArr,0,baseSize*sizeof(frame*));
		activeFrame = find(0);
		activeFrame = find(0);
}

/*find stored frame object for idx. If nothing found, return pointer
to new frame

should opto with sorting

caching eventually needed

*/
frame* find(int idx) {

		int i = 0;
		int found = 0;
		frame *ret;

		do{
//			printf("iterating frames idx:%d",i);
//			printf("%d\n",frameArr[i]);
			if(frameArr[i]==0) {
					found =-1;	/*we iterated the whole list and didn't find a frame*/

			} else {
					ret = frameArr[i];
					if(ret->idx == idx) {
							printf("found existing frame\n");
							return ret;
					}
					i++;
			}
		} while(found == 0);
		/*allocate a new frame*/
		printf("allocd new frame\n");
		createFrame(i, idx);

		/*frameArr[i] = malloc(sizeof(frame));
		frameArr[i]->idx = idx;
		frameArr[i]->drawing = createDrawingSurface(1920,1080);*/

		printf("%d - %d - %d" , frameArr[i], frameArr[i]->idx,i);
		return frameArr[i];
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

	printf("found left:%d\n",find_left());
}

void dropFrames(void) {
		int i;
		for(i=0; i<baseSize;++i) {
			if(frameArr[i]!=0) {
				SDL_FreeSurface(frameArr[i]->drawing);
			}
		}
		free(frameArr);
		destroyCompositeLayer(background);
		destroySurfaceCache();
}


