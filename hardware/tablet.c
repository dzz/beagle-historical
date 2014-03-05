#include <SDL.h>
#include <SDL_syswm.h>
#include <Windows.h>
#include <WindowsX.h>
#include <msgpack.h>
#include <wintab.h>
#define PACKETDATA (PK_X | PK_Y | PK_BUTTONS | PK_NORMAL_PRESSURE)
#define PACKETMODE PK_BUTTONS
#include <pktdef.h>


#include "tablet.h"
#include "../system/log.h"
#include "vendor/wintab_utils.h"
#include "../user/stylus.h"

char* gpszProgramName = "ctt2_windows_tablet";
HCTX hctx = 0;
AXIS gPressure = {0};
HWND gWnd;

void initTablet(SDL_Window* window) {
		HCTX hCtx;
		HWND hWnd;
		SDL_SysWMinfo data;

		LOGCONTEXTA lcMine = {{0}};
		UINT wWTInfoRetVal = 0;
		AXIS TabletX = {0};
		AXIS TabletY = {0};
		AXIS Pressure = {0};
		int numDevices = 0;
		int contextOpen;
		int ctxIndex = 0;
		int nOpenContexts = 0;
		int nAttachedDevices = 0;

		LoadWintab();
		if(!gpWTInfoA(0,0, NULL)) {
			log_msg("could not load wintab.");
			return;
		}

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
				gpWTClose(hctx);
				printf("dropped tablet context:%u",(unsigned int)hctx); 
		}
		UnloadWintab();
}

void handle_wt_packet(PACKET pkt) {
		int shouldUpdateStylus = 1;
		double pressureNorm = (double)pkt.pkNormalPressure / (double)gPressure.axMax;
		stylusPacket sPkt = {0};

		sPkt.x = pkt.pkX;
		sPkt.y = pkt.pkY;
		ScreenToClient(gWnd,(LPPOINT)&sPkt);
		sPkt.pressure = pressureNorm;

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
			if(gpWTPacket(hctx, wParam, &pkt)) {
				handle_wt_packet(pkt);
			}
	}
}
