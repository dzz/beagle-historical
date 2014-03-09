#ifndef __WINTAB_UTILS__
#define __WINTAB_UTILS__

#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

#include <WINTAB.H>

#include "../../system/log.h"

typedef UINT ( API * WTINFOA ) ( UINT, UINT, LPVOID );
typedef HCTX ( API * WTOPENA )( HWND, LPLOGCONTEXTA, BOOL );
typedef BOOL ( API * WTCLOSE ) ( HCTX );
typedef BOOL ( API * WTPACKET ) ( HCTX, UINT, LPVOID );

extern HINSTANCE handle_wintab;

extern WTOPENA _wt_OpenA;
extern WTCLOSE _wt_Close;
extern WTINFOA _wt_InfoA;
extern WTPACKET _wt_Packet;

BOOL initWintab( void );
void dropWintab( void );

#endif

