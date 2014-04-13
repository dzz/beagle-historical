//#define CTT2_SCREENMODE_DEBUG 


#include <conio.h>
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

/*debug sux*/
#undef _DEBUG

#include <Python.h>

#include <SDL.h>
#include <SDL_syswm.h>

#include "system/ctt2.h"
#include "ctt2_api.h"

#include "system/extended_video.h"
#include "system/wm_handler.h"
#include "system/log.h"
#include "system/surfaceCache.h"
#include "hardware/tablet.h"

#include "document/animation.h"

#include "drawing/hw_brush_context.h"
#include "drawing/brush.h"
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
static int drawingContextInvalid = 1;

static SDL_GLContext gl_context;

#define CTT2_SCREENMODE_DEBUG

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
    animation_cursor_move( 0, DO_NOT_COMMIT_DRAWING_CONTEXT );

}

/** DISPLAY MGMT **/

void updateViewingSurface() {
    SDL_GL_SwapWindow( opengl_window );
    glClear(GL_COLOR_BUFFER_BIT);
}

SDL_Surface* getViewingSurface(){
    return ui_surface;
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

    opengl_window = SDL_CreateWindow( "ctt2_hw", 50, 50, 
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


void dropPython(){
    if(PyErr_Occurred()) {
        PyErr_Print();
        printf("press a key...\n");
        getch();
        api_drop();
        Py_Finalize();
    }
}

void initPython() {
    Py_SetProgramName("ctt2_py");
    Py_Initialize();

    if( api_init() == API_FAILURE ) {
        dropPython();
        exit(1);
    }
}

/** MAIN **/

int main(int argc, char **argv){ 
    const int CYCLES_BETWEEN_SCREENBUFFER_UPDATES = 30;

    int screenbuffer_cycles = 0;

    int finished = 0;

    initLog();
    initPython();
    initDisplay();
    initWindowingSystemMessages();
    initOpenGL();
    initCompositor();
    initLayers();
    initAnimation();
    initHwBrush();
    initBrush();

    animation_cursor_move(0,DO_NOT_COMMIT_DRAWING_CONTEXT);

    initYankPut();
    initTablet(opengl_window);

    ui_surface = createDrawingSurface(SCREEN_WIDTH,SCREEN_HEIGHT);
    initPanels(ui_surface);

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


            if(screenbuffer_cycles++ > CYCLES_BETWEEN_SCREENBUFFER_UPDATES ) {
                frame* fr = getActiveFrame();
                screenbuffer_cycles = 0;
                hw_render_layerstack(fr);

                if( getPanelsEnabled() == PANELS_ENABLED ){
                    SDL_FillRect(ui_surface, NULL, SDL_MapRGBA( ui_surface->format, 0,0,0,0));
                    renderPanels(ui_surface);
                    renderLocalBuffer(ui_surface);
                }
                updateViewingSurface();
            }

            if(api_tick() == API_FAILURE) {
                finished = 1; 
            }
        }
    }
    /** FINISHED **/

    dropBrush();
    dropHwBrush();
    dropTablet();
    dropAnimation();
    dropPanels();
    dropDrawingSurfaces();
    dropYankPut();
    dropExtendedVideo();
    dropOpengl();
    dropDisplay();
    dropPython();
    dropLog();

    SDL_Quit();
    return 0;
}

