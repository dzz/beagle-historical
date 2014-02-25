#include "panels.h"
#include "../system/dirty.h"
#include "stylus.h"
#include "colorPicker.h"
#include "brushEditor.h"
#include "timeline.h"
#include <malloc.h>

int panelsEnabled = 1;

#define BRUSHPICKER_WIDTH 400
const int panelHeight = COLORPICKER_HEIGHT;
const int panelWidth = COLORPICKER_WIDTH + BRUSHPICKER_WIDTH;
const int screenHeight = 1080;
const int screenWidth = 1920;

UI_AREA *area;
UI_AREA *be_area;

void movePanel(int x, int y) {
	(*area).x0 = x;
	(*area).y0 = y;
	(*area).x1 = x+panelWidth;
	(*area).y1 = y+panelHeight;
	(*area).x = (*area).x0;
	(*area).y = (*area).y0;
	(*area).w = (*area).x1 - (*area).x0;
	(*area).h = (*area).y1 - (*area).y0;

	be_area->x = area->x+COLORPICKER_WIDTH;
	be_area->y = area->y;
	be_area->w = area->w-COLORPICKER_WIDTH;
	be_area->h = area->h;
}

void togglePanels(void) {
	stylusState ss = getStylusState();
	movePanel(ss.x,ss.y);
	panelsEnabled = !panelsEnabled;
	invalidateDirty(0,0,screenWidth,screenHeight);
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
	be_area = malloc(sizeof(UI_AREA));
	movePanel(0,0);

	panelColor = SDL_MapRGB(target->format,
					0xA8,
					0xA7,
					0xFF );

	initColorPicker();
	initBrushEditor();
	initTimeline();
}

static int mouse_x;
static int mouse_y;

#define PANEL_COLORPICKER 0
#define PANEL_BRUSHEDITOR 1

typedef struct {
	int offset_x;
	int offset_y;
	int panel_id;
} mouse_route;

void get_mouse_route(mouse_route* mr, int *x, int *y){
	if( *x < COLORPICKER_WIDTH ) {
		mr->offset_x = 0;
		mr->offset_y = 0;
		mr->panel_id = PANEL_COLORPICKER;
	} else {
		mr->offset_x = COLORPICKER_WIDTH;
		mr->offset_y = 0;
		mr->panel_id = PANEL_BRUSHEDITOR;
	}

	*x = *x - mr->offset_x;
	*y = *y - mr->offset_y;
}

void panels_dispatch_mousemotion(int x, int y) {
	mouse_route route;

	mouse_x = x;
	mouse_y = y;

	get_mouse_route(&route,&x,&y);


	switch(route.panel_id) {
			case PANEL_BRUSHEDITOR:
					brusheditor_mousemotion(x,y,area);
					colorpicker_mouseleave();
					break;
			case PANEL_COLORPICKER:
					colorpicker_mousemotion(x,y,area);
					brusheditor_mouseleave();
					break;
	}
}
void panels_dispatch_mouseup(int x,int y) {
	mouse_route route;
	get_mouse_route(&route,&x,&y);

	switch(route.panel_id) {
		case PANEL_COLORPICKER:
			colorpicker_mouseup(x,y,area);
			break;
		case PANEL_BRUSHEDITOR:
			brusheditor_mouseup(x,y,area);
			break;
	}
}

void panels_dispatch_mousedown(int x, int y) {
	mouse_route route;
	get_mouse_route(&route,&x,&y);

	switch(route.panel_id) {
		case PANEL_COLORPICKER:
			colorpicker_mousedown(x,y,area);
			break;
		case PANEL_BRUSHEDITOR:
			brusheditor_mousedown(x,y,area);
			break;
	}

}

void renderColorSwatch(SDL_Surface *target) {
		const int swatchWidth = 18;
		const int swatchHeight = 30;
		cp_color col = getPrimaryColor();
		SDL_Rect sr;

		sr.x = mouse_x + area->x;
		sr.y = mouse_y + area->y;

		sr.w = swatchWidth;
		sr.h = swatchHeight;

		SDL_FillRect( target, &sr, SDL_MapRGB(
								target->format,
								col.r,
								col.g,
								col.b ) );

		sr.x+=swatchWidth;
		col = getSecondaryColor();

		SDL_FillRect( target, &sr, SDL_MapRGB(
								target->format,
								col.r,
								col.g,
								col.b ) );

		invalidateDirty(sr.x-swatchWidth,sr.y,sr.x+swatchWidth,sr.y+swatchHeight);
}

void renderPanels(SDL_Surface *target) {
		if(panelsEnabled == 1) {
				renderColorSwatch(target);
				renderBrushEditor(target,be_area);
				renderColorPicker(target,area);
				renderTimeline(target);
		}

}

void dropPanels() {
	destroyColorPicker();
	destroyBrushEditor();
	if(area!=NULL){
		free(area);
		free(be_area);
	}
}
