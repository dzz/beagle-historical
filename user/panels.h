#ifndef __PANELS__
#define __PANELS__

#include "../user/stylus.h"
#include <SDL.H>


void togglePanels(void);
int getPanelsEnabled(void);
void panelsDispatchPacket(stylusPacket sPkt);

typedef struct {
	int x;
	int y;
	int w;
	int h;
	int x0;
	int y0;
	int x1;
	int y1;
} UI_AREA;

typedef struct {
	int x;
	int y;
} UI_MOUSE_POSITION;

UI_AREA getPanelsArea(void);
void renderPanels(SDL_Surface *target);
void initPanels(SDL_Surface *);
void dropPanels(void);

void panels_dispatch_mouseup(int x,int y);
void panels_dispatch_mousedown(int x,int y);
void panels_dispatch_mousemotion(int x,int y);
void panels_dispatch_mouseleave(void);
void panels_toggle_dragmode(void);
unsigned int panels_point_in_clients(int x, int y);

int pointInArea(int x, int y, UI_AREA area);
#endif

