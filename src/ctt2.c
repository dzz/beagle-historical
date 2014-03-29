//#define CTT2_SCREENMODE_DEBUG 

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#include <GL/wglext.h>
#endif

#ifdef __linux__
/*
 *
 *
 */
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

#include "drawing/hw_brush_context.h"
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

static SDL_Window* opengl_window;
static SDL_Surface* ui_surface = NULL;
static SDL_Surface* canvas_surface = NULL;
static int drawingContextInvalid = 1;

static SDL_GLContext gl_context;

#ifndef CTT2_SCREENMODE_DEBUG
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
#else
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
#endif

static unsigned int ctt2_keyframe_mode = 0;

/** KEYFRAMING FUNCTIONS **/

void toggleKeyframingMode() {
    ctt2_keyframe_mode = !ctt2_keyframe_mode;
}

unsigned int getKeyframingMode() {
    return ctt2_keyframe_mode;
}

void ctt2_insertkeyframe() {
    animation_insert_keyframe_at_cursor();
    animation_cursor_move( getDrawingContext(), 0, 
            DO_NOT_COMMIT_DRAWING_CONTEXT );

    invalidateDirty(0,0,1920,1080);
    updateDrawingContext();
}

/** DISPLAY MGMT **/

void updateViewingSurface() {
    SDL_GL_SwapWindow( opengl_window );
}

SDL_Surface* getViewingSurface(){
    return ui_surface;
}

void invalidateDrawingContext() {
    drawingContextInvalid = 1;
}


void updateDrawingContext() {
    SDL_Rect r = getDirtyRect();

    {
        
        SDL_Surface *comp = 
            compositeFrameWithContext( getDrawingContext() , getActiveFrame() , r);

        SDL_BlitSurface( comp,NULL, canvas_surface,&r);
        //importBrushContext(canvas_surface);
        SDL_FreeSurface(comp);
        
    }
    resetDirty();
    drawingContextInvalid = 0;
}

/** OPENGL HELPER **/

void disable_vsync()
{	
#ifdef _WIN32
    typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALPROC)( int );
    PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;
    wglSwapIntervalEXT = 
        (PFNWGLSWAPINTERVALPROC)SDL_GL_GetProcAddress( "wglSwapIntervalEXT" );
    wglSwapIntervalEXT(0);
#endif
#ifdef __linux__
    printf("linux unimplemented disable_vsync()");
#endif
}


void initWindowingSystemMessages() {
    SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
}

void initDisplay() {
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "%s\n", SDL_GetError() );
        exit(1);
    } 

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    opengl_window = SDL_CreateWindow( "ctt2_hw", 0, 0, 
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

    if( opengl_window == NULL ) {
        printf( "%s\n", SDL_GetError() );
        exit(1);
    }
}

void dropDisplay() {
    SDL_DestroyWindow( opengl_window);
}

void initOpenGL() {
    gl_context = SDL_GL_CreateContext(opengl_window);	
    disable_vsync();
    initExtendedVideo();
}

void dropOpengl() {
    SDL_GL_DeleteContext(gl_context);
}

/** MAIN **/

int main(int argc, char **argv){ 
    const int CYCLES_BETWEEN_RECOMPOSITE = 20;
    const int CYCLES_BETWEEN_SCREENBUFFER_UPDATES = 15;

    int recomposite_cycles = 0;
    int screenbuffer_cycles = 0;

    int finished = 0;

    initLog();
    initDisplay();
    initWindowingSystemMessages();
    initOpenGL();
    initCompositor();
    initLayers();
    initAnimation();
    initDrawingContext();
    initBrush( getDrawingContext() );
    animation_cursor_move(getDrawingContext(),0,DO_NOT_COMMIT_DRAWING_CONTEXT);
    initYankPut();
    initTablet(opengl_window);
    initHwBrush();

    ui_surface = createDrawingSurface(SCREEN_WIDTH,SCREEN_HEIGHT);
    canvas_surface = createDrawingSurface(1920,1080);

    initPanels(ui_surface);
    invalidateDirty(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);

    /** MAIN DISPATCH LOOP **/
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
            /*if(recomposite_cycles++ > CYCLES_BETWEEN_RECOMPOSITE ) {
                recomposite_cycles = 0;
                if(drawingContextInvalid == 1) {
                    updateDrawingContext();
                }
            }*/
            if(screenbuffer_cycles++ > CYCLES_BETWEEN_SCREENBUFFER_UPDATES ) {
                screenbuffer_cycles = 0;
                renderHwBrushContext();
                if( getPanelsEnabled() == PANELS_ENABLED ){
                    renderPanels(ui_surface);
                    renderLocalBuffer(ui_surface);
                }
                invalidateDrawingContext();
                updateViewingSurface();
            }
        }
    }
    /** FINISHED **/

    destroyBrush();
    dropHwBrush();
    dropTablet();
    dropDrawingContext();
    dropFrames();
    dropPanels();
    dropDrawingSurfaces();
    dropYankPut();
    dropExtendedVideo();
    dropOpengl();
    dropDisplay();
    dropLog();

    SDL_Quit();
    return 0;
}

