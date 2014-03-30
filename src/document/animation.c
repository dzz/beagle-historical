#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <SDL_image.h>

#include "animation.h"
#include "../system/ctt2.h"
#include "../system/surfaceCache.h"
#include "../drawing/drawingSurfaces.h"
#include "../drawing/hw_brush_context.h"
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
			frameArr[addr]->layerKeyFrames[i]= 0;
		}

		if(idx==0){
			for(i=0; i< numLayers; ++i) {
				frameArr[addr]->layerKeyFrames[i]= 1;
			}
		}
		allocateUninitializedLayers(frameArr[addr]);
		frameMap[idx] = addr;
}

void initAnimation(void) {

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
		animation_load();
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

		if( (idx_origin_plus_one) > (unsigned int)animation_total_frames ) {
			animation_total_frames = idx_origin_plus_one;
		}

        if(frameArr[frameMap[idx]] == 0){
				createFrame(findFreeAddr(), idx );
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

void animation_cursor_move(int delta, int commit) {

	COMPOSITE_LAYER* activeCompositeLayer = getCompositeLayerFromFrame( activeFrame, getActiveLayer(), COMP_RESOLVE_VIRTUAL	);

	if((activeFrame->idx==0) && (delta<0)) {
		return;
	}

	if(commit == 1) {
		hw_brush_commit_context();
	}

	activeFrame = find_implicit_create(activeFrame->idx+delta);
	/*copy current frame to context*/
	activeCompositeLayer = getCompositeLayerFromFrame( activeFrame, getActiveLayer(), COMP_RESOLVE_VIRTUAL );
    importBrushContext( activeCompositeLayer->data );
}

void dropAnimation(void) {
		free(frameArr);
		free(frameMap);
		destroyCompositeLayer(background);
		destroySurfaceCache();
}


typedef struct {
	unsigned int idx;
	unsigned int layer;
} keyframe_file_record;


const char *document_file_keyframe_index = "project-data\\keyframe_idx";
const char *document_image_output_pattern = "project-data\\%u_%u.png";
const char *export_image_output_pattern = "project-data\\export\\frame_%04u.png";

static void load_kfr(keyframe_file_record kfr) {
		
	char fname[2048];
	SDL_Surface* loadedImage;
	frame *fr;

	fr = find_implicit_create( kfr.idx );
    sprintf(fname,document_image_output_pattern,kfr.idx,kfr.layer);
	loadedImage = IMG_Load(fname);
	fr->layerKeyFrames[ kfr.layer ] = 1;
	allocateUninitializedLayers(fr);
	replaceLayerForExistingFrame(fr, kfr.layer, loadedImage );
	SDL_FreeSurface(loadedImage);
	printf("loaded frame %u layer %u\n",kfr.idx,kfr.layer);

}

void animation_insert_keyframe_at_cursor() {
	activeFrame->layerKeyFrames[ getActiveLayer() ] = 1;
	allocateUninitializedLayers(activeFrame);
}

void animation_delete_keyframe_at_cursor() {
	if( getActiveFrame()->idx !=0) {
		activeFrame->layerKeyFrames[ getActiveLayer() ] = 0;
		cleanupUnusedLayers(activeFrame);
	}
}

void animation_load() {
		FILE *kf_index_file = fopen(document_file_keyframe_index,"r");

		if (kf_index_file == 0 ) {
			return;
		}

		{
				keyframe_file_record kfr;
				for (;;) {
						fread(&kfr, sizeof(keyframe_file_record), 1, kf_index_file);
						printf("found frame in index: %u %u\n",kfr.idx,kfr.layer);
						//break out if we match a terminating record
						if( kfr.idx == 0xFFFFFFFF ) {
								printf("found terminator frame, finished loading index.\n");
								break;
						}
						load_kfr(kfr);
				}
				fclose(kf_index_file);
		}

} 

void animation_export() {
	unsigned int i;
	SDL_Rect export_area;

	export_area.x = 0;
	export_area.y = 0;
	export_area.w = 1920;
	export_area.h = 1080;

	for(i=0; i<animation_total_frames;++i) {
		char fname [1024];
		frame*fr = find_implicit_create(i);
		SDL_Surface *composited = compositeFrame(fr, export_area);
		sprintf( fname, export_image_output_pattern, i);
		IMG_SavePNG( composited, fname);
		SDL_FreeSurface(composited);
	}
}

void animation_preview() {
	unsigned int i;
	SDL_Rect render_area;

	render_area.x = 0;
	render_area.y = 0;
	render_area.w = 1920;
	render_area.h = 1080;
	for(i=0; i<animation_total_frames;++i) {
		frame*fr = find_implicit_create(i);

		SDL_Surface *composited = compositeFrame(fr, render_area);
		SDL_BlitSurface(composited,NULL,getViewingSurface(),NULL);
    renderLocalBuffer(getViewingSurface());
		updateViewingSurface();
		SDL_PumpEvents();
		SDL_FreeSurface(composited);
	}
}

void animation_save() {
		unsigned int i;
		FILE* kf_index_file = fopen(document_file_keyframe_index,"wb");

		for(i=0; i<animation_total_frames; ++i) {
				if( frame_has_content(i) == 1 ) {
						int j;
						frame* fr = find_implicit_create(i);
						char fname [2048];
						for(j=0; j<MAX_LAYERS; ++j) {
								if( fr->layerKeyFrames[j] == 1 ) {
										keyframe_file_record kfr;
										COMPOSITE_LAYER* image = getCompositeLayerFromFrame(fr,j,COMP_RESOLVE_ACTUAL);

										kfr.idx = i;
										kfr.layer = j;
										fwrite(&kfr, sizeof(keyframe_file_record), 1, kf_index_file);
										sprintf(fname,document_image_output_pattern,i,j);
										printf("%s\n",fname);
										IMG_SavePNG( image->data, fname);

								}
						}
				}
		}

		{
			keyframe_file_record terminator;
			terminator.idx = 0xFFFFFFFF;
			terminator.layer = 0xFFFFFFFF;
			fwrite(&terminator,sizeof(keyframe_file_record), 1, kf_index_file);
		}

		fclose(kf_index_file);
		exit(0);
}

