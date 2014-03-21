//#define CTT2_SCREENMODE_DEBUG 

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#include <GL/wglext.h>
#endif

#ifdef __linux__
//put whatever we need in here for
//getting at the platform specific way
//to disable vsync 
#endif

#include <SDL.h>
#include <SDL_syswm.h>

#include "system/ctt2.h"
#include "system/extended_video.h"
#include "system/wm_handler.h"
#include "system/log.h"
#include "system/dirty.h"
#include "system/surfaceCache.h"

#include "document/animation.h"

#include "drawing/hw_brush.h"
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
	SDL_Rect r = getDirtyRect();

	{
		SDL_Surface *comp = compositeFrameWithContext( getDrawingContext() , getActiveFrame() , r);
		SDL_BlitSurface( comp,NULL, screenSurface,&r);
		SDL_FreeSurface(comp);
	}
	resetDirty();
	drawingContextInvalid = 0;
}



SDL_Window* opengl_window;

void disable_vsync()
{	
#ifdef _WIN32
	typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALPROC)( int );
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)SDL_GL_GetProcAddress( "wglSwapIntervalEXT" );
	wglSwapIntervalEXT(0);
#endif
#ifdef __linux__
	printf("linux unimplemented disable_vsync()");
#endif
}

void initSDLSystems(SDL_Window** window,int SCREEN_WIDTH, int SCREEN_HEIGHT) {

		//Initialize SDL
		if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
				printf( "%s\n", SDL_GetError() );
				exit(1);
		} 

		
		//Create window
		*window = SDL_CreateWindow( "ctt2", 0, 0, 1920, 540, SDL_WINDOW_SHOWN );
		if( window == NULL ) {
				printf( "%s\n", SDL_GetError() );
				exit(1);
		}
		
		SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);


		//make an opengl window

		
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		opengl_window = SDL_CreateWindow( "ctt2", 0, 540, 
						1920, 540, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
		if( opengl_window == NULL ) {
				printf( "%s\n", SDL_GetError() );
				exit(1);
		}
}

int main(int argc, char **argv){ 
#ifndef CTT2_SCREENMODE_DEBUG
		const int SCREEN_WIDTH = 800;
		const int SCREEN_HEIGHT = 800;
#else
		const int SCREEN_WIDTH = 1920;
		const int SCREEN_HEIGHT = 400;
#endif
		const int CYCLES_BETWEEN_RECOMPOSITE = 20;
		const int CYCLES_BETWEEN_SCREENBUFFER_UPDATES = 3;

		int recomposite_cycles = 0;
		int screenbuffer_cycles = 0;

		int finished = 0;

		SDL_GLContext gl_context;

		initLog();
		initSDLSystems(&window, SCREEN_WIDTH,SCREEN_HEIGHT);
		initCompositor();
		initLayers();
		initAnimation();
		initDrawingContext();
		initBrush( getDrawingContext() );
		animation_cursor_move(getDrawingContext(),0,DO_NOT_COMMIT_DRAWING_CONTEXT);
		initYankPut();
		initTablet(opengl_window);

		gl_context = SDL_GL_CreateContext(opengl_window);	
		disable_vsync();
		initExtendedVideo();
		initHwBrush();

		screenSurface = SDL_GetWindowSurface( window );
		//screenSurface = createDrawingSurface(1920,1080);
		SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00 ) );

		initPanels(screenSurface);
		invalidateDirty(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);

		{
				unsigned int render_gui = 0;
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
												render_gui = 1;
												break;
										case SDL_KEYUP:
												render_gui = 0;
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
								SDL_GL_SwapWindow(opengl_window);
						}
				}
		}

		destroyBrush();
		dropHwBrush();
		dropTablet();
		dropDrawingContext();
		dropFrames();
		dropPanels();
		dropDrawingSurfaces();
		dropYankPut();
		dropExtendedVideo();
		dropLog();
		SDL_GL_DeleteContext(gl_context);
		SDL_DestroyWindow( opengl_window);
		SDL_DestroyWindow( window );
		SDL_Quit();
		return 0;
}

/* A simple function that will read a file into an allocated char pointer buffer */
char* read_file(char *file)
{
    FILE *fptr;
    long length;
    char *buf;
 
    fptr = fopen(file, "rb"); /* Open file for reading */
    if (!fptr) /* Return NULL on failure */
        return NULL;
    fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
    length = ftell(fptr); /* Find out how many bytes into the file we are */
    buf = (char*)malloc(length+1); /* Allocate a buffer for the entire length of the file and a null terminator */
    fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
    fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
    fclose(fptr); /* Close the file */
    buf[length] = 0; /* Null terminator */

    return buf; /* Return the buffer */
}
