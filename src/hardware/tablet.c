#include <SDL.h>
#include <SDL_syswm.h>

#ifdef _WIN32

#include <Windows.h>
#include <WindowsX.h>
#include <msgpack.h>
#include <wintab.h>
#define PACKETDATA (PK_X | PK_Y | PK_BUTTONS | PK_NORMAL_PRESSURE | PK_TIME | PK_ORIENTATION )
#define PACKETMODE PK_BUTTONS
#include <pktdef.h>
#include "vendor/wintab_utils.h"

#endif


#include "../user/stylus.h"
#include "../user/panels.h"
#include "../system/log.h"

#include "hw_run_vars.h"


#include "tablet.h"

#ifdef _WIN32

HCTX hctx = 0;
AXIS pressure_axis = {0};
AXIS orient_axis[3] = {0};

HWND window_handle;

void initTablet(SDL_Window* window) {
		HCTX hCtx;
		HWND hWnd;
		SDL_SysWMinfo data;

		LOGCONTEXTA lcMine = {{0}};
		AXIS TabletX = {0};
		AXIS TabletY = {0};
		AXIS Pressure = {0};
		AXIS Orient[3] = {0};
		int numDevices = 0;
		int contextOpen;
		int ctxIndex = 0;
		int nOpenContexts = 0;
		int nAttachedDevices = 0;

		initWintab();
		if(!_wt_InfoA(0,0, NULL)) {
			log_msg("could not load wintab.");
			return;
		}

		SDL_VERSION(&data.version);
		SDL_GetWindowWMInfo(window,&data);
		hWnd = data.info.win.window;
		window_handle = hWnd;

		_wt_InfoA(WTI_INTERFACE, IFC_NDEVICES, &numDevices);
		{
				printf("Getting info on contextIndex: %i ...\n", ctxIndex);
				contextOpen = _wt_InfoA(WTI_DDCTXS + ctxIndex, 0, &lcMine);

				if ( contextOpen > 0 )
				{
		                unsigned int status = 0;
						lcMine.lcPktData = PACKETDATA;
						lcMine.lcOptions |= CXO_MESSAGES;
						lcMine.lcOptions |= CXO_SYSTEM;
						lcMine.lcPktMode = PACKETMODE;
						lcMine.lcMoveMask = PACKETDATA;
						lcMine.lcBtnUpMask = lcMine.lcBtnDnMask;

						// Set the entire tablet as active
						_wt_InfoA( WTI_DEVICES + ctxIndex, DVC_X, &TabletX );
						_wt_InfoA( WTI_DEVICES + ctxIndex, DVC_Y, &TabletY );

						_wt_InfoA( WTI_DEVICES + ctxIndex, DVC_NPRESSURE, &Pressure );
						_wt_InfoA( WTI_DEVICES + ctxIndex, DVC_ORIENTATION, &orient_axis );

						lcMine.lcInOrgX = 0;
						lcMine.lcInOrgY = 0;
						lcMine.lcInExtX = TabletX.axMax;
						lcMine.lcInExtY = TabletY.axMax;

                        //set x,y to screen coords
						lcMine.lcOutOrgX = GetSystemMetrics( SM_XVIRTUALSCREEN );
						lcMine.lcOutOrgY = GetSystemMetrics( SM_YVIRTUALSCREEN );
						lcMine.lcOutExtX = GetSystemMetrics( SM_CXVIRTUALSCREEN );
						lcMine.lcOutExtY = -GetSystemMetrics( SM_CYVIRTUALSCREEN ); // invert

						// Leave the system origin and extents as received:
						// lcSysOrgX, lcSysOrgY, lcSysExtX, lcSysExtY

						// Open the context enabled.
						hCtx = _wt_OpenA(hWnd, &lcMine, TRUE);
						if ( hCtx )
						{
							hctx = hCtx;
							pressure_axis = Pressure;
    						printf("context:%u\n",(unsigned int)hctx);
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

		//i don't think we need to check this
		if ( nOpenContexts < nAttachedDevices ) {
				log_msg("nOpenContexts < nAttachedDevices");
		}
		return;
}


void dropTablet() { 
		if(hctx>0) {
				_wt_Close(hctx);
				printf("dropped tablet context:%u",(unsigned int)hctx); 
		}
		dropWintab();
}

void handle_wt_packet(PACKET pkt) {
		int shouldUpdateStylus = 1;
		double pressureNorm = (double)pkt.pkNormalPressure / (double)pressure_axis.axMax;
        double orientNorm = (double)pkt.pkOrientation.orAzimuth / (double)orient_axis[0].axMax;

		stylusPacket sPkt = {0};

		sPkt.x = pkt.pkX;
		sPkt.y = pkt.pkY;
		ScreenToClient(window_handle,(LPPOINT)&sPkt);
		sPkt.pressure = pressureNorm;
		sPkt.timestamp = pkt.pkTime;
        sPkt.azimuth = orientNorm;

		if (getPanelsEnabled() == 1) {
				if(panels_point_in_clients(sPkt.x,sPkt.y)==1){
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
			HW_RUN_VAR_TABLET_CONNECTED = TABLET_CONNECTED;
			if(_wt_Packet(hctx, wParam, &pkt)) {
				handle_wt_packet(pkt);
			}
	}
}

#endif

#ifdef __linux__

void initTablet(SDL_Window* window) {};
void dropTablet() {};
void handle_wm_event(SDL_Event event) {};

#endif

