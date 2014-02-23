#include "dispatch.h"
#include "colorPicker.h"
#include "../document/animation.h"
#include "panels.h"

int pointInArea(int x, int y, UI_AREA area) {
	if( (x > area.x0) &&
		(x < area.x1) &&
		(y > area.y0) &&
		(y < area.y1) ) {
			return 1;
		}
	return 0;
}
/* this is where we'll dispatch to the rest of the ui */
/* mode is 0 for keyup 1 for keydown */
void dispatch_key(SDL_Keycode sym, int mode) {
		switch(sym) {
			case SDLK_z:
					if(mode==1) 
							togglePanels();
					break;
			case SDLK_s:
					if(mode==1)
							animation_save();
					break;
		}
}


void dispatch_mousemotion(int x, int y) {
	if( getPanelsEnabled() ) {
		UI_AREA panelArea = getPanelsArea();
		/*if(pointInArea(x,y,panelArea) == 1)*/	{
			x -= panelArea.x;
			y -= panelArea.y;
			panels_dispatch_mousemotion(x,y);	
		}
	}
}
void dispatch_mousedown( int button,int x,int y) {
	if( getPanelsEnabled() ) {
		UI_AREA panelArea = getPanelsArea();
		if(pointInArea(x,y,panelArea) == 1)	{
			x -= panelArea.x;
			y -= panelArea.y;
			panels_dispatch_mousedown(x,y);	
		}
	}
}

void dispatch_mouseup(int button,int x, int y) {
	if( getPanelsEnabled() ) {
		UI_AREA panelArea = getPanelsArea();
		if(pointInArea(x,y,panelArea) == 1)	{
			x -= panelArea.x;
			y -= panelArea.y;
			panels_dispatch_mouseup(x,y);		
		}
	}
}
