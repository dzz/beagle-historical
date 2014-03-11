#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_syswm.h>

#include "system/ctt2.h"
#include "system/wm_handler.h"
#include "system/log.h"
#include "system/dirty.h"
#include "system/surfaceCache.h"

#include "document/animation.h"

#include "drawing/brush.h"
#include "drawing/drawingContext.h"
#include "drawing/drawingSurfaces.h"

#include "compositor/compositor.h"

#include "document/layers.h"

#include "user/stylus.h"
#include "user/panels.h"
#include "user/dispatch.h"
#include "user/yank_put.h"

#include "user/editors/colorPicker.h"

static SDL_Window *window = NULL;
static SDL_Surface *screenSurface = NULL;

static int drawingContextInvalid = 1;

void updateViewingSurface() {
	SDL_UpdateWindowSurface( window );
}

SDL_Surface* getViewingSurface(){
	return screenSurface;
}

static unsigned int ctt2_keyframe_mode = 0;

// should live in animation, these are just hacks for now
void toggleKeyframingMode() {
	ctt2_keyframe_mode = !ctt2_keyframe_mode;
}

unsigned int getKeyframingMode() {
	return ctt2_keyframe_mode;
}

void ctt2_insertkeyframe() {
		animation_insert_keyframe_at_cursor();
		animation_cursor_move( getDrawingContext(), 0, DO_NOT_COMMIT_DRAWING_CONTEXT );
		invalidateDirty(0,0,1920,1080);
		updateDrawingContext();
}
void invalidateDrawingContext() {
	drawingContextInvalid = 1;
}

void updateDrawingContext() {
	SDL_Rect r = getDirtyRect();\

	{
		SDL_Surface *comp = compositeFrameWithContext( getDrawingContext() , getActiveFrame() , r);
		SDL_BlitSurface( comp,NULL, screenSurface,&r);
		SDL_FreeSurface(comp);
	}
	resetDirty();
	drawingContextInvalid = 0;
}


void initSDLSystems(SDL_Window** window,int SCREEN_WIDTH, int SCREEN_HEIGHT) {
		//Initialize SDL
		if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
				printf( "%s\n", SDL_GetError() );
				exit(1);
		} 
		//Create window
		*window = SDL_CreateWindow( "ctt2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL ) {
				printf( "%s\n", SDL_GetError() );
				exit(1);
		}
		
		SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
}

int main(int argc, char **argv){ 
#ifndef CTT2_SCREENMODE_DEBUG
		const int SCREEN_WIDTH = 1920;
		const int SCREEN_HEIGHT = 1080;
#else
		const int SCREEN_WIDTH = 920;
		const int SCREEN_HEIGHT = 1080;
#endif
		const int CYCLES_BETWEEN_RECOMPOSITE = 20;
		const int CYCLES_BETWEEN_SCREENBUFFER_UPDATES = 3;

		int recomposite_cycles = 0;
		int screenbuffer_cycles = 0;

		int finished = 0;

		initLog();
		initSDLSystems(&window, SCREEN_WIDTH,SCREEN_HEIGHT);
		initCompositor();
		initLayers();
		initAnimation();
		initDrawingContext();
		initBrush( getDrawingContext() );
		animation_cursor_move(getDrawingContext(),0,DO_NOT_COMMIT_DRAWING_CONTEXT);
		initYankPut();
		initTablet(window);

		screenSurface = SDL_GetWindowSurface( window );
		SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00 ) );
		SDL_UpdateWindowSurface( window );

		initPanels(screenSurface);
		invalidateDirty(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);

		{
				SDL_Event event;
				while(finished == 0) {
						if(SDL_PollEvent(&event)) {
								switch (event.type) {
										case SDL_QUIT:
												finished = 1;
												break;
										case SDL_SYSWMEVENT:
												handle_wm_event(event);
												break;
										case SDL_KEYDOWN:
												finished = dispatch_key(event.key.keysym.sym,1);
												break;
										case SDL_KEYUP:
												dispatch_key(event.key.keysym.sym,0);
												break;
										case SDL_MOUSEBUTTONDOWN:
												dispatch_mousedown(event.button.button,
																event.button.x,
																event.button.y );
												break;
										case SDL_MOUSEBUTTONUP:
												dispatch_mouseup(event.button.button,
																event.button.x,
																event.button.y );
										case SDL_MOUSEMOTION:
												dispatch_mousemotion(event.motion.x, 
																event.motion.y );
												break;

								}
						}
						recomposite_cycles++;
						if(recomposite_cycles > CYCLES_BETWEEN_RECOMPOSITE ) {
								recomposite_cycles = 0;
								if(drawingContextInvalid == 1) {
										updateDrawingContext();
								}
						}
						screenbuffer_cycles++;
						if(screenbuffer_cycles > CYCLES_BETWEEN_SCREENBUFFER_UPDATES ) {
								screenbuffer_cycles = 0;
								renderPanels(screenSurface);
								invalidateDrawingContext();
								updateViewingSurface();
						}
				}
		}
		SDL_DestroyWindow( window );
		destroyBrush();
		dropTablet();
		dropDrawingContext();
		dropFrames();
		dropPanels();
		dropDrawingSurfaces();
		dropYankPut();
		dropLog();
		SDL_Quit();
		return 0;
}

