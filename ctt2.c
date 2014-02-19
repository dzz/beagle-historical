//#define WACOM_DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <WindowsX.h>
#include <msgpack.h>
#include <wintab.h>
#define PACKETDATA (PK_X | PK_Y | PK_BUTTONS | PK_NORMAL_PRESSURE)
#define PACKETMODE PK_BUTTONS
#include <pktdef.h>
#include <SDL.h>
#include <SDL_syswm.h>

#include "system/ctt2.h"
#include "system/dirty.h"
#include "system/surfaceCache.h"

#include "vendor/wintab_utils.h"

#include "user/stylus.h"
#include "document/animation.h"

#include "drawing/brush.h"
#include "drawing/drawingSurfaces.h"

#include "compositor/compositor.h"

#include "document/layers.h"

#include "user/panels.h"
#include "user/colorPicker.h"
#include "user/dispatch.h"

HWND gWnd;
char* gpszProgramName = "ctt22";
SDL_Surface *drawingContext;
SDL_Surface *screenSurface = NULL;
HCTX hctx = 0;
AXIS gPressure = {0};
int drawingContextInvalid = 1;
void postBootstrap(void);
FILE *logfile;

void startLog() {
	logfile = fopen("log.txt","w+");
}

FILE *getLogfile(){
	return logfile;
}

void closeLog() {
	fclose(logfile);
}

__declspec( dllexport) void __stdcall origin_init() {
		LoadWintab();
		if(!gpWTInfoA(0,0, NULL)) {
			printf("fail");
			return;
		}
		makewin();
		UnloadWintab();
		return;
}

void initDrawingContext() {
	drawingContext = createDrawingSurface(1920,1080);
	initBrush(drawingContext);
}

void dropDrawingContext() {
	SDL_FreeSurface(drawingContext);
}

void invalidateDrawingContext() {
	drawingContextInvalid = 1;
}

static char field_mode = 0;
void updateDrawingContext() {
	field_mode = (field_mode +1) % 2;

	SDL_Rect r = getDirtyRect();
	{
		SDL_Surface *comp = compositeFrameWithContext( drawingContext, getActiveFrame() , r ,field_mode );
		SDL_BlitSurface( comp,NULL, screenSurface,&r);
		SDL_FreeSurface(comp);
	}
	resetDirty();
	drawingContextInvalid = 0;
}

void initTablet(SDL_Window* window) {
		HCTX hCtx;
		HWND hWnd;
		SDL_SysWMinfo data;

		LOGCONTEXTA lcMine = {0};
		UINT wWTInfoRetVal = 0;
		AXIS TabletX = {0};
		AXIS TabletY = {0};
		AXIS Pressure = {0};
		int numDevices = 0;
		int contextOpen;
		int ctxIndex = 0;
		int nOpenContexts = 0;
		int nAttachedDevices = 0;

		/* grab the hWnd */ /*move somewhere smarter*/
		SDL_VERSION(&data.version);
		SDL_GetWindowWMInfo(window,&data);
		hWnd = data.info.win.window;
		gWnd = hWnd;

		gpWTInfoA(WTI_INTERFACE, IFC_NDEVICES, &numDevices);
		{
				printf("Getting info on contextIndex: %i ...\n", ctxIndex);
				contextOpen = gpWTInfoA(WTI_DDCTXS + ctxIndex, 0, &lcMine);

				if ( contextOpen > 0 )
				{
						lcMine.lcPktData = PACKETDATA;
						lcMine.lcOptions |= CXO_MESSAGES;
						lcMine.lcOptions |= CXO_SYSTEM;
						lcMine.lcPktMode = PACKETMODE;
						lcMine.lcMoveMask = PACKETDATA;
						lcMine.lcBtnUpMask = lcMine.lcBtnDnMask;

						// Set the entire tablet as active
						wWTInfoRetVal = gpWTInfoA( WTI_DEVICES + ctxIndex, DVC_X, &TabletX );
						if (  wWTInfoRetVal != sizeof( AXIS ) )
						{
								WacomTrace("This context should not be opened.\n");
								return;
						}

						wWTInfoRetVal = gpWTInfoA( WTI_DEVICES + ctxIndex, DVC_Y, &TabletY );

						gpWTInfoA( WTI_DEVICES + ctxIndex, DVC_NPRESSURE, &Pressure );
						WacomTrace("Pressure: %i, %i\n", Pressure.axMin, Pressure.axMax);

						lcMine.lcInOrgX = 0;
						lcMine.lcInOrgY = 0;
						lcMine.lcInExtX = TabletX.axMax;
						lcMine.lcInExtY = TabletY.axMax;

						// Guarantee the output coordinate space to be in screen coordinates.
						lcMine.lcOutOrgX = GetSystemMetrics( SM_XVIRTUALSCREEN );
						lcMine.lcOutOrgY = GetSystemMetrics( SM_YVIRTUALSCREEN );
						lcMine.lcOutExtX = GetSystemMetrics( SM_CXVIRTUALSCREEN );

						// In Wintab, the tablet origin is lower left.  Move origin to upper left
						// so that it coincides with screen origin.
						lcMine.lcOutExtY = -GetSystemMetrics( SM_CYVIRTUALSCREEN );

						// Leave the system origin and extents as received:
						// lcSysOrgX, lcSysOrgY, lcSysExtX, lcSysExtY

						// Open the context enabled.
						hCtx = gpWTOpenA(hWnd, &lcMine, TRUE);
						if ( hCtx )
						{
							hctx = hCtx;
							gPressure = Pressure;
hjkjk:							printf("context:%u\n",hctx);
						}
						else
						{
								return;
						}
				}
				else
				{
						return;
				}

				ctxIndex++;
		}

		if ( nOpenContexts < nAttachedDevices ) {
				ShowError("Oops - did not open a context for each attached device");
		}
		return;
}

void dropTablet() { if(hctx>0) {gpWTClose(hctx);printf("dropped tablet context:%u",hctx); }}


void handle_wt_packet(PACKET pkt) {
		int shouldUpdateStylus = 1;
		double pressureNorm = (double)pkt.pkNormalPressure / (double)gPressure.axMax;
		stylusPacket sPkt = {0};

		sPkt.x = pkt.pkX;
		sPkt.y = pkt.pkY;
		ScreenToClient(gWnd,(LPPOINT)&sPkt);
		sPkt.pressure = pressureNorm;

		if (getPanelsEnabled() == 1) {
				UI_AREA panelArea = getPanelsArea();
				if( (sPkt.x > panelArea.x0) &&
								(sPkt.y > panelArea.y0) &&
								(sPkt.x < panelArea.x1) &&
								(sPkt.y < panelArea.y1) ) {
						panelsDispatchPacket(sPkt);
						shouldUpdateStylus = 0;
				}
		}
		if(shouldUpdateStylus == 1) {
				updateStylus(sPkt);
				invalidateDrawingContext();
		} else {
			resetStylusState();
		}
}

void handle_wm_event(SDL_Event event) {
	PACKET pkt;
	UINT msg = event.syswm.msg->msg.win.msg;
	WPARAM wParam = event.syswm.msg->msg.win.wParam;
	LPARAM lParam = event.syswm.msg->msg.win.lParam;

	if(msg == WT_PACKET) {
			if(gpWTPacket(hctx, wParam, &pkt)) {
				handle_wt_packet(pkt);
			}
	}
}

int local_dispatch(SDL_Keycode sym);

__declspec( dllexport) void __stdcall makewin() {
		SDL_Event event;
		const int SCREEN_WIDTH = 500;
		const int SCREEN_HEIGHT = 500;

		/* controls how many mainloops pass before an invalid context is
		 * reupdated, for a small performance increase */
		const int CYCLES_BETWEEN_RECOMPOSITE = 12;
		const int CYCLES_BETWEEN_SCREENBUFFER_UPDATES = 2;

		int recomposite_cycles = 0;
		int screenbuffer_cycles = 0;

		SDL_Window* window = NULL;
		int fin = 0;

		startLog();

		//Initialize SDL
		if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
				printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		} else {
				//Create window
				window = SDL_CreateWindow( "ctt2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
				if( window == NULL ) {
						printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
				}
				else {
						SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
						initCompositor();
						initLayers();
						initFrames();
						initDrawingContext();
						initTablet(window);

						screenSurface = SDL_GetWindowSurface( window );
						SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
						SDL_UpdateWindowSurface( window );
				}
		}

		postBootstrap();

		while(!fin) {
				if(SDL_PollEvent(&event)) {
						switch (event.type) {
								case SDL_QUIT:
										fin = 1;
										break;
								case SDL_SYSWMEVENT:
										handle_wm_event(event);
										break;
								case SDL_KEYDOWN:
										dispatch_key(event.key.keysym.sym,1);
										fin = local_dispatch(event.key.keysym.sym);
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
										dispatch_mousemotion(event.motion.x, event.motion.y );
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
					SDL_UpdateWindowSurface( window );
				}
		}

		SDL_DestroyWindow( window );
		dropTablet();
		dropDrawingContext();
		dropFrames();
		dropPanels();
		dropDrawingSurfaces();
		SDL_Quit();
		closeLog();
}
//low level hook for debugging, return 1 to signal
// program exit

int local_dispatch(SDL_Keycode sym) {
	switch(sym) {
			case SDLK_p:
					return 1;
			case SDLK_q:
					anim_nav(drawingContext,-1, 1);
					invalidateDirty(0,0,1920,1080);
					break;
			case SDLK_e:
					anim_nav(drawingContext,1, 1);
					invalidateDirty(0,0,1920,1080);
					break;
			case SDLK_w:

					anim_nav(drawingContext,0, 1);
					setActiveLayer(1);
					anim_nav(drawingContext,0, 0);
					invalidateDirty(0,0,1920,1080);
					break;
			case SDLK_s:

					anim_nav(drawingContext,0, 1);
					setActiveLayer(0);
					anim_nav(drawingContext,0, 0);
					invalidateDirty(0,0,1920,1080);
					break;
			case SDLK_c:
					cp_toggle_primary_secondary();
					break;
	}
	updateDrawingContext();
	return 0;
}

int main(int argc, char **argv){ origin_init(); return 0; }

void postBootstrap(void) {
	initPanels(screenSurface);
	invalidateDirty(0,0,1920,1080);
}
