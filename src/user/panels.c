#include <malloc.h>
#include <math.h>

#include "../system/dirty.h"
#include "../system/ctt2.h"

#include "panels.h"
#include "dispatch.h"
#include "stylus.h"

#include "editors/colorPicker.h"
#include "editors/brushEditor.h"
#include "editors/timeline.h"
#include "editors/toolbar.h"
#include "editors/nodeMapEditor.h"

int panelsEnabled = 1;

#define BRUSHEDITOR_WIDTH 256
#define BRUSHEDITOR_HEIGHT 256

static unsigned int dragging_panel_id = -1;

static UI_AREA *area;
static UI_AREA *brusheditor_area;
static UI_AREA *colorpicker_area;
static UI_AREA *toolbar_area;
static UI_AREA *nodemapeditor_area;

#define TOTAL_PANELS 4
#define PANEL_COLORPICKER 0
#define PANEL_BRUSHEDITOR 1
#define PANEL_TOOLBAR 2
#define PANEL_NODEMAPEDITOR 3

static UI_AREA* route_map[TOTAL_PANELS];
static mouse_handler mouse_handlers[TOTAL_PANELS];

void initUIAreas() {
#define NEW_AREA (UI_AREA*)malloc(sizeof(UI_AREA));
    area = NEW_AREA;
    colorpicker_area = NEW_AREA;
    brusheditor_area = NEW_AREA;
    toolbar_area = NEW_AREA;
    nodemapeditor_area = NEW_AREA;
#undef NEW_AREA

    route_map[PANEL_COLORPICKER] = colorpicker_area;
    route_map[PANEL_BRUSHEDITOR] = brusheditor_area;
    route_map[PANEL_TOOLBAR] = toolbar_area;
    route_map[PANEL_NODEMAPEDITOR] = nodemapeditor_area;
}

void dummy_xy_mousehandler(int x, int y) {}
void dummy_void_mousehandler() {}

void bind_mouse_handlers() {

    //the spec for handlers does not include passing the
    //UI_AREA for the sub panel, and everything seems to be
    //working so may want to clean up those function
    //declarations. The x,y's passed should be relative
    //to the mouse_route so the sub panels shouldn't need that
    //context

    mouse_handlers[PANEL_COLORPICKER].bound_mousedown_handler = &colorpicker_mousedown;
    mouse_handlers[PANEL_COLORPICKER].bound_mouseup_handler = &colorpicker_mouseup;
    mouse_handlers[PANEL_COLORPICKER].bound_mouseleave_handler = &colorpicker_mouseleave;
    mouse_handlers[PANEL_COLORPICKER].bound_mousemotion_handler = &colorpicker_mousemotion;

    mouse_handlers[PANEL_BRUSHEDITOR].bound_mousedown_handler = &brusheditor_mousedown;
    mouse_handlers[PANEL_BRUSHEDITOR].bound_mouseup_handler = &brusheditor_mouseup;
    mouse_handlers[PANEL_BRUSHEDITOR].bound_mouseleave_handler = &brusheditor_mouseleave;
    mouse_handlers[PANEL_BRUSHEDITOR].bound_mousemotion_handler = &brusheditor_mousemotion;


    mouse_handlers[PANEL_TOOLBAR].bound_mousedown_handler = &toolbar_mousedown;
    mouse_handlers[PANEL_TOOLBAR].bound_mouseup_handler = &dummy_xy_mousehandler;
    mouse_handlers[PANEL_TOOLBAR].bound_mouseleave_handler = &dummy_void_mousehandler;
    mouse_handlers[PANEL_TOOLBAR].bound_mousemotion_handler = &dummy_xy_mousehandler;

    mouse_handlers[PANEL_NODEMAPEDITOR].bound_mousedown_handler = &nodemapeditor_mousedown;
    mouse_handlers[PANEL_NODEMAPEDITOR].bound_mouseup_handler = &nodemapeditor_mouseup;
    mouse_handlers[PANEL_NODEMAPEDITOR].bound_mouseleave_handler = &nodemapeditor_mouseleave;
    mouse_handlers[PANEL_NODEMAPEDITOR].bound_mousemotion_handler = &nodemapeditor_mousemotion;
}


void normalize_UI_area_extra_vars( UI_AREA *p) {
    p->x0 = p->x;
    p->x1 = p->x+p->w;
    p->y0 = p->y;
    p->y1 = p->y+p->h;
}

void layoutPanels() {
    //TODO create a layout manager to autostack these
    // want the user to be able to organize these at
    // somepoint
    (*area).x0 = 0;
    (*area).y0 = 0;
    (*area).x1 = 1366;
    (*area).y1 = 768;
    (*area).x = (*area).x0;
    (*area).y = (*area).y0;
    (*area).w = (*area).x1 - (*area).x0;
    (*area).h = (*area).y1 - (*area).y0;

    colorpicker_area->x = 1366-COLORPICKER_WIDTH;
    colorpicker_area->y = area->y;
    colorpicker_area->w = COLORPICKER_WIDTH;
    colorpicker_area->h = COLORPICKER_HEIGHT;

    brusheditor_area->x = 1366-BRUSHEDITOR_WIDTH;
    brusheditor_area->y = colorpicker_area->y+colorpicker_area->h;
    brusheditor_area->w = area->w-colorpicker_area->w;
    brusheditor_area->h = BRUSHEDITOR_HEIGHT;

    toolbar_area->x = 1366 -TOOLBAR_WIDTH;
    toolbar_area->y = brusheditor_area->y+brusheditor_area->h;
    /*toolbar_area->w = TOOLBAR_WIDTH;
      toolbar_area->h = 48;*/
    toolbar_area->w=0;
    toolbar_area->h=0;


    nodemapeditor_area->x=0;
    nodemapeditor_area->y=0;
    nodemapeditor_area->h=768;
    nodemapeditor_area->w=1366;

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
    invalidateDirty(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
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

void initPanels() {
    initUIAreas();
    bind_mouse_handlers();
    initColorPicker();
    initBrushEditor();
    initTimeline();
    initToolbar();
    initNodeMapEditor();
    layoutPanels();
}

typedef struct {
    int offset_x;
    int offset_y;
    int panel_id;
} mouse_route;

void get_mouse_route(mouse_route* mr, int *x, int *y){
    int i;
    mr->panel_id=-1;
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
        if(route.panel_id!=-1)
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
    if( route.panel_id!=-1)
        mouse_handlers[route.panel_id].bound_mouseup_handler(x,y);
}

void panels_dispatch_mousedown(int x, int y) {
    if(dragmode) {
        panels_dispatch_mousedown_dragmode(x,y);
    } else {
        mouse_route route;
        get_mouse_route(&route,&x,&y);
        if( route.panel_id!=-1)
            mouse_handlers[route.panel_id].bound_mousedown_handler(x,y);
    }
}


void renderPanels(SDL_Surface *target) {
    if(panelsEnabled == 1) {
        if( dragmode == 0 ) {
            renderNodeMapEditor(target,nodemapeditor_area);
            //renderColorSwatch(target);
            renderBrushEditor(target,brusheditor_area);
            renderColorPicker(target,colorpicker_area);
            renderTimeline(target);
            //renderToolbar(target,toolbar_area);
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
    panelsEnabled = 0;
    destroyColorPicker();
    destroyBrushEditor();
    //destroyToolbar();
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
