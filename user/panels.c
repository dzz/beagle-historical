#include <malloc.h>
#include <math.h>

#include "../system/dirty.h"

#include "panels.h"
#include "dispatch.h"
#include "stylus.h"
#include "colorPicker.h"
#include "brushEditor.h"
#include "timeline.h"
#include "mapperEditorBank.h"
#include "toolbar.h"
#include "nodeMapEditor.h"

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
static UI_AREA *nodemapeditor_area;

#define TOTAL_PANELS 5
#define PANEL_COLORPICKER 0
#define PANEL_BRUSHEDITOR 1
#define PANEL_MAPPERBANK 2
#define PANEL_TOOLBAR 3
#define PANEL_NODEMAPEDITOR 4

static unsigned int dragging_panel_id = -1;

static UI_AREA* route_map[TOTAL_PANELS];
static mouse_handler mouse_handlers[TOTAL_PANELS];

void initUIAreas() {
	#define NEW_AREA (UI_AREA*)malloc(sizeof(UI_AREA));
	area = NEW_AREA;
	colorpicker_area = NEW_AREA;
	brusheditor_area = NEW_AREA;
	mapperbank_area = NEW_AREA;
	toolbar_area = NEW_AREA;
	nodemapeditor_area = NEW_AREA;
	#undef NEW_AREA

	route_map[PANEL_COLORPICKER] = colorpicker_area;
	route_map[PANEL_BRUSHEDITOR] = brusheditor_area;
	route_map[PANEL_MAPPERBANK] = mapperbank_area;
	route_map[PANEL_TOOLBAR] = toolbar_area;
	route_map[PANEL_NODEMAPEDITOR] = nodemapeditor_area;
}

void dummy_xy_mousehandler(int x, int y) {}
void dummy_void_mousehandler() {}

void bind_mouse_handlers() {
	mouse_handlers[PANEL_COLORPICKER].bound_mousedown_handler = &colorpicker_mousedown;
	mouse_handlers[PANEL_COLORPICKER].bound_mouseup_handler = &colorpicker_mouseup;
	mouse_handlers[PANEL_COLORPICKER].bound_mouseleave_handler = &colorpicker_mouseleave;
	mouse_handlers[PANEL_COLORPICKER].bound_mousemotion_handler = &colorpicker_mousemotion;

	mouse_handlers[PANEL_BRUSHEDITOR].bound_mousedown_handler = &brusheditor_mousedown;
	mouse_handlers[PANEL_BRUSHEDITOR].bound_mouseup_handler = &brusheditor_mouseup;
	mouse_handlers[PANEL_BRUSHEDITOR].bound_mouseleave_handler = &brusheditor_mouseleave;
	mouse_handlers[PANEL_BRUSHEDITOR].bound_mousemotion_handler = &brusheditor_mousemotion;

	mouse_handlers[PANEL_MAPPERBANK].bound_mousedown_handler = &mapperbank_mousedown;
	mouse_handlers[PANEL_MAPPERBANK].bound_mouseup_handler = &mapperbank_mouseup;
	mouse_handlers[PANEL_MAPPERBANK].bound_mouseleave_handler = &mapperbank_mouseleave;
	mouse_handlers[PANEL_MAPPERBANK].bound_mousemotion_handler = &mapperbank_mousemotion;

	mouse_handlers[PANEL_TOOLBAR].bound_mousedown_handler = &toolbar_mousedown;
	mouse_handlers[PANEL_TOOLBAR].bound_mouseup_handler = &dummy_xy_mousehandler;
	mouse_handlers[PANEL_TOOLBAR].bound_mouseleave_handler = &dummy_void_mousehandler;
	mouse_handlers[PANEL_TOOLBAR].bound_mousemotion_handler = &dummy_xy_mousehandler;

	mouse_handlers[PANEL_NODEMAPEDITOR].bound_mousedown_handler = &nodemapeditor_mousedown;
	mouse_handlers[PANEL_NODEMAPEDITOR].bound_mouseup_handler = &dummy_xy_mousehandler;
	mouse_handlers[PANEL_NODEMAPEDITOR].bound_mouseleave_handler = &dummy_void_mousehandler;
	mouse_handlers[PANEL_NODEMAPEDITOR].bound_mousemotion_handler = &dummy_xy_mousehandler;
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
	mapperbank_area->h = mapperbank_get_height();

	toolbar_area->x = 800;
	toolbar_area->y = 0;
	toolbar_area->w = TOOLBAR_WIDTH;
	toolbar_area->h = 48;
	
	nodemapeditor_area->x=(1920/2)-200;
	nodemapeditor_area->y=400;
	nodemapeditor_area->h=500;
	nodemapeditor_area->w=500;

	//normalize our convenience variables
	{
		int i;
		for(i = 0; i< TOTAL_PANELS; ++i) {
			normalize_UI_area_extra_vars(route_map[i]);
		}
	}
}

void togglePanels(void) {
	panelsEnabled = !panelsEnabled;
	invalidateDirty(0,0,screenWidth,screenHeight);
}

static unsigned int dragmode = 0;
static unsigned int dragstart_x = 0;
static unsigned int dragstart_y = 0;

void panels_enable_dragmode() {
	dragmode = 1;
}

void panels_disable_dragmode() {
	dragmode = 0;
	dragging_panel_id = -1;
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
	bind_mouse_handlers();
	initColorPicker();
	initBrushEditor();
	initTimeline();
	initMapperEditorBank();
	initToolbar();
	movePanel(0,0);
}

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

unsigned int panels_point_in_clients(int x, int y){
		int i;
		for(i=0; i<TOTAL_PANELS; ++i) 
		{
			if(pointInArea( x + area->x, y + area->y, *route_map[i]) == 1 ) 
			{
					return 1;
			}
		}
		return 0;
}


void execute_drag(int x, int y) {
	invalidateDirty(
					route_map[dragging_panel_id]->x0,
					route_map[dragging_panel_id]->y0,
					route_map[dragging_panel_id]->x1,
					route_map[dragging_panel_id]->y1);

	route_map[dragging_panel_id]->x+=(client_get_screen_mousex()-dragstart_x);
	route_map[dragging_panel_id]->y+=(client_get_screen_mousey()-dragstart_y);
	dragstart_x = client_get_screen_mousex();
	dragstart_y = client_get_screen_mousey();

	normalize_UI_area_extra_vars(route_map[dragging_panel_id]);
}

static int mousemotion_previous_panel = -1;
void panels_dispatch_mouseleave() {
		if( mousemotion_previous_panel != -1 ) {
			mouse_handlers[mousemotion_previous_panel].bound_mouseleave_handler();
		}
}

void panels_dispatch_mousemotion(int x, int y) {
	mouse_route route;
	get_mouse_route(&route,&x,&y);


	if(dragging_panel_id != -1) {
		execute_drag(x,y);
	}

	mouse_handlers[route.panel_id].bound_mousemotion_handler(x,y);

	if(mousemotion_previous_panel != route.panel_id && 
	   mousemotion_previous_panel != -1) {
		mouse_handlers[mousemotion_previous_panel].bound_mouseleave_handler();
	}

	mousemotion_previous_panel = route.panel_id;
}

void panels_dispatch_mousedown_dragmode(int x, int y) {
		mouse_route route;
		dragstart_x = client_get_screen_mousex();
		dragstart_y = client_get_screen_mousey();
		get_mouse_route(&route,&x,&y);
		dragging_panel_id = route.panel_id;
}

void panels_dispatch_mouseup(int x,int y) {
	mouse_route route;
	get_mouse_route(&route,&x,&y);

	if(dragmode) {
		dragging_panel_id = -1;
		return;
	}
	mouse_handlers[route.panel_id].bound_mouseup_handler(x,y);
}

void panels_dispatch_mousedown(int x, int y) {
		if(dragmode) {
				panels_dispatch_mousedown_dragmode(x,y);
		} else {
				mouse_route route;
				get_mouse_route(&route,&x,&y);
				mouse_handlers[route.panel_id].bound_mousedown_handler(x,y);
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
				if( dragmode == 0 ) {
						renderColorSwatch(target);
						renderBrushEditor(target,brusheditor_area);
						renderColorPicker(target,colorpicker_area);
						renderTimeline(target);
						renderMapperEditorBank(target,mapperbank_area);
						renderToolbar(target,toolbar_area);
						renderNodeMapEditor(target,nodemapeditor_area);
				} else {
						int i;
						for(i=0; i<TOTAL_PANELS; ++i) {
								unsigned int color = SDL_MapRGB(target->format,
												i*30,(10-i)*20,128-(i*40));
								SDL_FillRect(target,route_map[i],color);
						}
				}
		}
}

void dropPanels() {
	destroyColorPicker();
	destroyBrushEditor();
	destroyMapperEditorBank();
	destroyToolbar();
	destroyNodeMapEditor();
	{
		int i;
		for(i=0; i< TOTAL_PANELS; ++i) {
			free(route_map[i]);
		}
	}	
}

int pointInArea(int x, int y, UI_AREA area) {
	if( (x > area.x0) &&
		(x < area.x1) &&
		(y > area.y0) &&
		(y < area.y1) ) {
			return 1;
		}
	return 0;
}
