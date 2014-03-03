#include <malloc.h>

#include "../system/dirty.h"

#include "panels.h"
#include "dispatch.h"
#include "stylus.h"
#include "colorPicker.h"
#include "brushEditor.h"
#include "timeline.h"
#include "mapperEditorBank.h"


int panelsEnabled = 1;

#define BRUSHEDITOR_WIDTH 256
#define BRUSHEDITOR_HEIGHT 256

const int screenHeight = 1080;
const int screenWidth = 1920;
const int panelHeight = COLORPICKER_HEIGHT + MAPPER_BANK_HEIGHT;
const int panelWidth = COLORPICKER_WIDTH + BRUSHEDITOR_WIDTH;


static UI_AREA *area;
static UI_AREA *brusheditor_area;
static UI_AREA *mapperbank_area;
static UI_AREA *colorpicker_area;
static UI_AREA *toolbar_area;

#define TOTAL_PANELS 4
#define PANEL_COLORPICKER 0
#define PANEL_BRUSHEDITOR 1
#define PANEL_MAPPERBANK 2
#define PANEL_TOOLBAR 3

static UI_AREA* route_map[TOTAL_PANELS];
void initUIAreas() {
	#define NEW_AREA (UI_AREA*)malloc(sizeof(UI_AREA));
	area = NEW_AREA;
	colorpicker_area = NEW_AREA;
	brusheditor_area = NEW_AREA;
	mapperbank_area = NEW_AREA;
	toolbar_area = NEW_AREA;
	#undef NEW_AREA

	route_map[PANEL_COLORPICKER] = colorpicker_area;
	route_map[PANEL_BRUSHEDITOR] = brusheditor_area;
	route_map[PANEL_MAPPERBANK] = mapperbank_area;
	route_map[PANEL_TOOLBAR] = toolbar_area;
}


void normalize_UI_area_extra_vars( UI_AREA *p) {
	p->x0 = p->x;
	p->x1 = p->x+p->w;
	p->y0 = p->y;
	p->y1 = p->y+p->h;
}

void movePanel(int x, int y) {
	//TODO create a layout manager to autostack these
	// want the user to be able to organize these at
	// somepoint
	(*area).x0 = x;
	(*area).y0 = y;
	(*area).x1 = x+panelWidth;
	(*area).y1 = y+panelHeight;
	(*area).x = (*area).x0;
	(*area).y = (*area).y0;
	(*area).w = (*area).x1 - (*area).x0;
	(*area).h = (*area).y1 - (*area).y0;

	colorpicker_area->x = area->x;
	colorpicker_area->y = area->y;
	colorpicker_area->w = COLORPICKER_WIDTH;
	colorpicker_area->h = COLORPICKER_HEIGHT;

	brusheditor_area->x = area->x+colorpicker_area->w;
	brusheditor_area->y = area->y;
	brusheditor_area->w = area->w-colorpicker_area->w;
	brusheditor_area->h = BRUSHEDITOR_HEIGHT;

	mapperbank_area->x = area->x;
	mapperbank_area->y = area->y+colorpicker_area->h;
	mapperbank_area->w = area->w;
	mapperbank_area->h = area->h;

	//normalize our convenience variables
	{
		int i;
		for(i = 0; i< TOTAL_PANELS; ++i) {
			normalize_UI_area_extra_vars(route_map[i]);
		}
	}
}

void togglePanels(void) {
	stylusState ss = getStylusState();
	//movePanel(ss.x,ss.y);
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



void initPanels(SDL_Surface *target) {
	initUIAreas();
	movePanel(0,0);
	initColorPicker();
	initBrushEditor();
	initTimeline();
	initMapperEditorBank();
	initToolbar();
}

static int mouse_x;
static int mouse_y;


typedef struct {
	int offset_x;
	int offset_y;
	int panel_id;
} mouse_route;

void get_mouse_route(mouse_route* mr, int *x, int *y){
		int i;
		for(i=0; i<TOTAL_PANELS; ++i) 
		{
			if(pointInArea( *x + area->x, *y + area->y, *route_map[i]) == 1 ) 
			{
				mr->offset_x = route_map[i]->x - area->x;
				mr->offset_y = route_map[i]->y - area->y;
				mr->panel_id = i;
				break;	
			}
		}

		*x = *x - mr->offset_x;
		*y = *y - mr->offset_y;
}

void panels_dispatch_mouseleave() {
	brusheditor_mouseleave();
	colorpicker_mouseleave();
}

void panels_dispatch_mousemotion(int x, int y) {
	mouse_route route;

	mouse_x = x;
	mouse_y = y;

	get_mouse_route(&route,&x,&y);


	switch(route.panel_id) {
			case PANEL_BRUSHEDITOR:
					brusheditor_mousemotion(x,y,area);
					/**/
					colorpicker_mouseleave();
					mapperbank_mouseleave();
					break;
			case PANEL_COLORPICKER:
					colorpicker_mousemotion(x,y,area);

					brusheditor_mouseleave();
					mapperbank_mouseleave();
					break;
			case PANEL_MAPPERBANK:
					mapperbank_mousemotion(x,y,area);

					colorpicker_mouseleave();
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
		case PANEL_MAPPERBANK:
			mapperbank_mouseup(x,y,area);
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
		case PANEL_MAPPERBANK:
			mapperbank_mousedown(x,y,area);
			break;
		case PANEL_TOOLBAR:
			toolbar_mousedown(x,y,area);
			break;
	}

}

void renderColorSwatch(SDL_Surface *target) {
		const int swatchWidth = 18;
		const int swatchHeight = 30;
		cp_color col = getPrimaryColor();
		SDL_Rect sr;

		sr.x = client_get_screen_mousex();
		sr.y = client_get_screen_mousey();

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
				renderBrushEditor(target,brusheditor_area);
				renderColorPicker(target,area);
				renderTimeline(target);
				renderMapperEditorBank(target,mapperbank_area);
				renderToolbar(target,toolbar_area);
		}
}

void dropPanels() {
	destroyColorPicker();
	destroyBrushEditor();
	destroyMapperEditorBank();
	destroyToolbar();

	if(area!=NULL){
		free(area);
		free(brusheditor_area);
		free(mapperbank_area);
	}
}
