#include "panels.h"
#include "../system/dirty.h"
#include "stylus.h"
#include "colorPicker.h"
#include "brushEditor.h"

int panelsEnabled = 1;

#define BRUSHPICKER_WIDTH 400
const int panelHeight = COLORPICKER_HEIGHT;
const int panelWidth = COLORPICKER_WIDTH + BRUSHPICKER_WIDTH;
const int screenHeight = 1080;
const int screenWidth = 1920;

UI_AREA *area;

void movePanel(int x, int y) {
	(*area).x0 = x;
	(*area).y0 = y; 
	(*area).x1 = x+panelWidth;
	(*area).y1 = y+panelHeight;
	(*area).x = (*area).x0;
	(*area).y = (*area).y0;
	(*area).w = (*area).x1 - (*area).x0;
	(*area).h = (*area).y1 - (*area).y0;
}

void togglePanels(void) {
	stylusState ss = getStylusState();
	panelsEnabled = !panelsEnabled;
	invalidateDirty(0,0,screenWidth,screenHeight);
	movePanel(ss.x,ss.y);
}

int getPanelsEnabled(void) {
	return panelsEnabled;
}

void panelsDispatchPacket(stylusPacket sPkt) {
	//in case we want to use pressure sensitive stylus data
	//in the panel UI
}


UI_AREA getPanelsArea(void) {
		return (*area);
}

unsigned int panelColor;

void initPanels(SDL_Surface *target) {
	area = malloc(sizeof(UI_AREA));
	movePanel(0,0);

	panelColor = SDL_MapRGB(target->format,
					0xA8,
					0xA7,
					0xFF );

	initColorPicker();
}

static int mouse_x;
static int mouse_y;

void panels_dispatch_mousemotion(int x, int y) {
	mouse_x = x;
	mouse_y = y;
}

#define PANEL_COLORPICKER 0
#define PANEL_BRUSHEDITOR 1 

typedef struct {
	int offset_x;
	int offset_y;
	int panel_id;	
} mouse_route;

void get_mouse_route(mouse_route* mr, int x, int y){
	if(x < COLORPICKER_WIDTH ) {
		mr->offset_x = 0;
		mr->offset_y = 0;
		mr->panel_id = PANEL_COLORPICKER;
	} else {
		mr->offset_x = COLORPICKER_WIDTH;
		mr->offset_y = 0;
		mr->panel_id = PANEL_BRUSHEDITOR;
	}
}

void panels_dispatch_mouseup(int x,int y) {
	mouse_route route;
	get_mouse_route(&route,x,y);

	switch(route.panel_id) {
		case PANEL_COLORPICKER:
			colorpicker_mousedown(x,y,area);
			break;
		case PANEL_BRUSHEDITOR:
			break;
	}
}

void panels_dispatch_mousedown(int x, int y) {

}

void renderColorSwatch(SDL_Surface *target) {
		cp_color col = getPrimaryColor();
		SDL_Rect sr;

		sr.x = mouse_x - 30 - 3; 
		sr.y = mouse_y - 30 - 3;

		sr.w = 15;
		sr.h = 30;

		SDL_FillRect( target, &sr, SDL_MapRGB(
								target->format,
								col.r,
								col.g,
								col.b ) );

		sr.x+=15;
		col = getSecondaryColor();

		SDL_FillRect( target, &sr, SDL_MapRGB(
								target->format,
								col.r,
								col.g,
								col.b ) );

		invalidateDirty(sr.x-15,sr.y,sr.x+30,sr.y+30);
}

void renderPanels(SDL_Surface *target) {
		if(panelsEnabled == 1) {
				SDL_Rect * r = (SDL_Rect*)area; //steady now
				SDL_FillRect(target,r,panelColor);
				renderColorPicker(target,area);
				renderBrushEditor(target,area);
				renderColorSwatch(target);		
		}

}

void dropPanels() {
	destroyColorPicker();
	if(area!=NULL)
		free(area);
}
