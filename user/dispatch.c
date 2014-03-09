#include "../system/dirty.h"
#include "../system/ctt2.h"
#include "../document/animation.h"
#include "../user/yank_put.h"
#include "../hardware/hw_run_vars.h"

#include "dispatch.h"
#include "colorPicker.h"
#include "panels.h"

#define KEYMODE_DOWN 1
#define KEYMODE_UP 0

SYSTEM_SIGNAL dispatch_key(SDL_Keycode sym, int mode) {
		if(mode==KEYMODE_UP) {
				switch(sym) {
						case SDLK_SPACE:
								panels_disable_dragmode();
				}
		}

		if(mode==KEYMODE_DOWN) {
				//keydown handlers
				switch(sym) {
						case SDLK_0:
								animation_preview();
								break;
						case SDLK_ESCAPE:
								return SYSSIG_QUIT_CTT2;
						case SDLK_SPACE:
								panels_enable_dragmode();
								break;
						case SDLK_TAB:
								togglePanels();
								break;
						case SDLK_s:
								animation_save();
								break;
						case SDLK_c:
								cp_toggle_primary_secondary();
								break;
						case SDLK_k:
								animation_insert_keyframe_at_cursor();
								animation_cursor_move( getDrawingContext(), 0, DO_NOT_COMMIT_DRAWING_CONTEXT );
								invalidateDirty(0,0,1920,1080);
								updateDrawingContext();
								break;
						case SDLK_d:
								animation_delete_keyframe_at_cursor();
								animation_cursor_move( getDrawingContext(), 0, DO_NOT_COMMIT_DRAWING_CONTEXT );
								invalidateDirty(0,0,1920,1080);
								updateDrawingContext();
								break;
						case SDLK_x:
								animation_export();
								break;
						case SDLK_r:
								if(getKeyframingMode() != KEYFRAME_MODE_RECORD) {
									//hax plz 2 fix
									ctt2_insertkeyframe();
								}
								toggleKeyframingMode();
								break;
						case SDLK_q:
								animation_cursor_move(getDrawingContext(),-1, 
												COMMIT_DRAWING_CONTEXT);
								if( getKeyframingMode() == KEYFRAME_MODE_RECORD)
										ctt2_insertkeyframe();
								invalidateDirty(0,0,1920,1080);
								break;
						case SDLK_e:
								animation_cursor_move(getDrawingContext(),1, 
												COMMIT_DRAWING_CONTEXT);
								if( getKeyframingMode() == KEYFRAME_MODE_RECORD)
										ctt2_insertkeyframe();
								invalidateDirty(0,0,1920,1080);
								break;
						case SDLK_1:
								animation_cursor_move(getDrawingContext(),0, 
												COMMIT_DRAWING_CONTEXT);
								setActiveLayer(0);
								animation_cursor_move(getDrawingContext(),0, 0);
								invalidateDirty(0,0,1920,1080);
								break;
						case SDLK_2:
								animation_cursor_move(getDrawingContext(),0, 
												COMMIT_DRAWING_CONTEXT);
								setActiveLayer(1);
								animation_cursor_move(getDrawingContext(),0, 0);
								invalidateDirty(0,0,1920,1080);
								break;
						case SDLK_y:
								yankDrawingContext();
								break;
						case SDLK_p:
								putDrawingContext();
								break;

				}
		}
		return 0;
}

static int client_mousex = 0;
static int client_mousey = 0;
int client_get_screen_mousex() { return client_mousex;}
int client_get_screen_mousey() { return client_mousey;}

static double emulated_stylus_pressure = 0;

stylusPacket _spacket(int x, int y, double pressure ){
		stylusPacket sp;
		sp.x = x;
		sp.y = y;
		sp.pressure = pressure;
		emulated_stylus_pressure = pressure;
		return sp;
}


void dispatch_mousemotion(int x, int y) {
		client_mousex = x;
		client_mousey = y;

		if( getPanelsEnabled() && 
						panels_point_in_clients(x,y) ) {
				panels_dispatch_mousemotion(x,y);	
		} else {
				panels_dispatch_mouseleave();
				if( HW_RUN_VAR_TABLET_CONNECTED == TABLET_NOT_CONNECTED ) {
						updateStylus( _spacket( x,y, emulated_stylus_pressure ) );
				}	
		}
}

void dispatch_mousedown( int button,int x,int y) {
		if( getPanelsEnabled() && 
						panels_point_in_clients(x,y) )	{
				panels_dispatch_mousedown(x,y);	
		} else {
				if( HW_RUN_VAR_TABLET_CONNECTED == TABLET_NOT_CONNECTED ) {
						updateStylus( _spacket( x,y, 1 ) );
				}	
		}
}

void dispatch_mouseup(int button,int x, int y) {
		if( getPanelsEnabled() && panels_point_in_clients(x,y) ){
				panels_dispatch_mouseup(x,y);		
		} 

		if( HW_RUN_VAR_TABLET_CONNECTED == TABLET_NOT_CONNECTED ) {
				updateStylus( _spacket( x,y, 0 ) );
		}	
}
