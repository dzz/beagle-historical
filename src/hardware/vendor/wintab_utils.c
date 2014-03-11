#ifdef _WIN32

#include "wintab_utils.h"

HINSTANCE handle_wintab = NULL;
WTINFOA _wt_InfoA = NULL;
WTOPENA _wt_OpenA = NULL;
WTCLOSE _wt_Close = NULL;
WTPACKET _wt_Packet = NULL;

#define LOAD_WINTAB_FUNCTION(target, type, wt_func )\
	target = (type) GetProcAddress( handle_wintab, #wt_func);\
	if(!target){ dropWintab(); log_msg("failed to load wintab function"); return FALSE; }
	
BOOL initWintab( void )
{
	handle_wintab = LoadLibraryA( "Wintab32.dll" );
	
	if ( !handle_wintab )
	{
		DWORD err = GetLastError();
		printf("LoadLibrary error: %i\n", err);
		printf("Could not load Wintab32.dll");
		return FALSE;
	}

	LOAD_WINTAB_FUNCTION( _wt_OpenA, WTOPENA, WTOpenA );
	LOAD_WINTAB_FUNCTION( _wt_Close, WTOPENA, WTClose );
	LOAD_WINTAB_FUNCTION( _wt_InfoA, WTOPENA, WTInfoA );
	LOAD_WINTAB_FUNCTION( _wt_Packet, WTOPENA, WTPacket );

	return TRUE;
}

void dropWintab( void )
{
	if ( handle_wintab ) {
		FreeLibrary( handle_wintab );
		handle_wintab = NULL;
	}

	_wt_OpenA = NULL;
	_wt_Close = NULL;
	_wt_InfoA = NULL;
	_wt_Packet = NULL;
}

#endif
