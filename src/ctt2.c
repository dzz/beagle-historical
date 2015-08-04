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
//todo: tablet / vsync
#endif
/*undef _DEBUG to get around py linking issues */
#undef _DEBUG
#include <Python.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_keycode.h>
#include "system/ctt2.h"
#include "system/ctt2_host.h"
#include "drawing/node_resource_ids.h"
#include "system/extended_video.h"
#include "system/wm_handler.h"
#include "system/log.h"
#include "system/surfaceCache.h"
#include "hardware/tablet.h"
#include "hardware/hf_timer.h"
#include "hardware/gamepad.h"
#include "hardware/audio.h"
#include "document/animation.h"
#include "drawing/shader_brush.h"
#include "drawing/brush.h"
#include "drawing/drawingSurfaces.h"
#include "compositor/compositor.h"
#include "document/layers.h"
#include "user/stylus.h"
#include "user/panels.h"
#include "user/dispatch.h"
#include "user/yank_put.h"
#include "user/editors/colorPicker.h"
#include "hwgfx/context.h"
#include "hwgfx/text.h"

//not static due to reference in the host api for
//host_set_title
SDL_Window* opengl_window; 

static SDL_Surface* ui_surface = NULL;
static int drawingContextInvalid = 1;

static SDL_GLContext gl_context;

//#define CTT2_SCREENMODE_DEBUG

int SCREEN_WIDTH = 1200;
int SCREEN_HEIGHT = 700;

/**************************************/

static unsigned int ctt2_keyframe_mode = 0;

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

/**************************************/

void updateViewingSurface() {
    SDL_GL_SwapWindow( opengl_window );
    //glClear(GL_COLOR_BUFFER_BIT);
}

SDL_Surface* getViewingSurface(){
    return ui_surface;
}

/**************************************/

void vsync(int mode)
{	
#ifdef _WIN32
    typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALPROC)( int );
    PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;
    wglSwapIntervalEXT = 
        (PFNWGLSWAPINTERVALPROC)SDL_GL_GetProcAddress( "wglSwapIntervalEXT" );
    wglSwapIntervalEXT(mode);
#endif
#ifdef __linux__
    printf("linux unimplemented vsync()");
#endif
}

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
//SDL_ShowCursor(0);
}

/**************************************/


void initDisplay( int fullscreen) {
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER ) < 0 ) {
        printf( "%s\n", SDL_GetError() );
        exit(1);
    } 

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    if(fullscreen == 1 ) {
        opengl_window = SDL_CreateWindow( "ctt2_hw", 64, 64, 
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP );
    } else {
        opengl_window = SDL_CreateWindow( "ctt2_hw", 64, 64, 
                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN  );
    }




    if( opengl_window == NULL ) {
        printf( "%s\n", SDL_GetError() );
        exit(1);
    }
}

void dropDisplay() {
    SDL_DestroyWindow( opengl_window);
}

void DIRTY_DISPLAY_ABORT() {
    dropDisplay();
}

/**************************************/

void initOpenGL() {
    gl_context = SDL_GL_CreateContext(opengl_window);	
    //disable_vsync();
    vsync(1);
    initExtendedVideo();
}

void dropOpengl() {
    printf("Dropping ogl\n");
    SDL_GL_DeleteContext(gl_context);
}

/**************************************/

void dropPython(){
    if(PyErr_Occurred()) {
        PyErr_Print();
        printf("press a key...\n");
        _getch();
    }
    api_drop();
    printf("CH:Py_Finalize()\n");
    Py_Finalize();
}

void initPython() {
    char buffer[1024];
    Py_SetProgramName("ctt2_py");
    if( api_init() == API_FAILURE ) {
        dropPython();
        exit(1);
    } 



}


/*****************************************************************************/

void initTextInput() {
    SDL_StartTextInput();
}

void dropTextInput() {
    SDL_StopTextInput();
}


#define CTT2_NO_EVT         0
#define CTT2_EVT_TICK       1
#define CTT2_EVT_RENDER     2
#define CTT2_EVT_SYNC_GFX   3


int main(int argc, char **argv){ 
    const int CYCLES_BETWEEN_SCREENBUFFER_UPDATES   = 1;
    int screenbuffer_cycles                         = 20;

    int finished                                    = 0;
    //int resizable                                   = 0;
    int fullscreen                                  = 0;
    int fps                                         = -1;
    double frame_millis                             = -1;
    double init_millis                              = 0;
    double tick_millis                              = 0;
    double frame_overflow                           = 0;
    double spf                                      = 0.0;
    int tick_next                                   = 0;
    unsigned int ctt2_state                         = CTT2_NO_EVT;

    if(argc==5) {
        SCREEN_WIDTH    = atoi( argv[1] );
        SCREEN_HEIGHT   = atoi( argv[2] );
        fullscreen      = atoi( argv[3] );
        fps             = atoi( argv[4] );
        spf = 1.0/(double)fps;
        frame_millis    = (double)1000/(double)fps;
        //printf("frame millis:%f", frame_millis);
    }




    initLog();    
	  

    initDisplay(fullscreen);
    initAudio();
    initWindowingSystemMessages();
    initOpenGL();
    initTextInput();
    initHfTimer();

    initGamepad();

    ui_surface = createDrawingSurface(SCREEN_WIDTH,
                                        SCREEN_HEIGHT);

    api_set_screensize( SCREEN_WIDTH, SCREEN_HEIGHT );

    init_millis = getTimeMs(); 

    initPython();


    /** MAIN DISPATCH LOOP **/
    {
        SDL_Event event;
        double base_millis = getTimeMs();
        tick_millis = getTimeMs();


		queue:
        while(finished == 0) {
            /*
            if(tick_next == 1)  {
                if(api_tick() == API_FAILURE) { finished = 1; }
                tick_next = 0;
                //audio_garbage_collect_channels();
            }*/

            switch(ctt2_state) {
                    case CTT2_EVT_TICK:
                        if(api_tick() == API_FAILURE) { 
                                finished = 1; 
                            } else {
                                ctt2_state = CTT2_EVT_RENDER;
                                tick_millis = getTimeMs();
                            }
						 break;
                    case CTT2_EVT_RENDER:
                         api_render();
						 updateViewingSurface();  
                         ctt2_state = CTT2_NO_EVT;
						 break;
                    case CTT2_NO_EVT:
                         if( (getTimeMs() - tick_millis) > frame_millis ) {
                            ctt2_state = CTT2_EVT_TICK;
                         } else {
							 Sleep(0);
						 }
						  break;
            }


            while(SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_CONTROLLERDEVICEADDED:
                        dropGamepad();
                        initGamepad();
                        break;
                    case SDL_CONTROLLERDEVICEREMOVED:
                        dropGamepad();
                        initGamepad();
                        break;
                    case SDL_JOYAXISMOTION:
                        GamepadHandleEvent( &event );
                        break; 
                    case SDL_TEXTINPUT:
                        api_dispatch_text( event.text.text );
                        break;
                    case SDL_WINDOWEVENT:
                        /*resize window*/
                        if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                        {
                            viewport_dims vd;
                            vd.x = 0;
                            vd.y = 0;
                            vd.w = event.window.data1;
                            vd.h = event.window.data2;
                            SCREEN_WIDTH=vd.w;
                            SCREEN_HEIGHT=vd.h;
                            gfx_viewport_set_dims(vd);
                            resizeExtendedVideo();
                        }
                        break;
                    case SDL_QUIT:
                        finished = 1;
                        break;
                    case SDL_SYSWMEVENT:
                       // handle_wm_event(event);
                        break;
                    case SDL_KEYDOWN:
                        if( event.key.keysym.sym == SDLK_F5 && (event.key.keysym.mod & KMOD_CTRL) ) {
                            dropPython();
                            initPython();
                        }
                        finished = dispatch_key(event.key.keysym.sym,1);
                        if( api_dispatch_key(event.key.keysym.sym,1) 
                                == API_FAILURE ) finished = 1;
                        break;
                    case SDL_KEYUP:
                        dispatch_key(event.key.keysym.sym,0);
                        if( api_dispatch_key(event.key.keysym.sym,0) 
                                == API_FAILURE ) finished = 1;
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        /*dispatch_mousedown(event.button.button,
                                event.button.x,
                                event.button.y );*/
                        if(api_dispatch_mousedown(
                                    event.button.button, 
                                    event.button.x, 
                                    event.button.y) == API_FAILURE ) 
                                        finished = 1;
                        break;
                    case SDL_MOUSEBUTTONUP:
                        /*dispatch_mouseup(event.button.button,
                                event.button.x,
                                event.button.y );*/
                        if(api_dispatch_mouseup(
                                    event.button.button, 
                                    event.button.x, 
                                    event.button.y) == API_FAILURE ) 
                                        finished = 1;
                        break;
                    case SDL_MOUSEMOTION:
                        /*dispatch_mousemotion(event.motion.x, 
                                event.motion.y );*/

                        if(api_dispatch_mousemotion(
                                    event.motion.x, 
                                    event.motion.y) == API_FAILURE ) 
                                        finished = 1;
                        break;
                }
            }




        }
    }
    /** FINISHED **/
    dropDrawingSurfaces();
    dropGamepad();
	dropAudio(); //drop audio before python 
    dropPython();
    dropTextInput();
    dropExtendedVideo();
    dropOpengl();
    dropDisplay();
    dropLog();
    SDL_Quit();
    return 0;
}
