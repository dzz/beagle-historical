#include <SDL.h>
#include <math.h>
#include <malloc.h>

#include "../../system/ctt2.h"

#include "../../drawing/node_mapper.h"
#include "../../drawing/node_resource_ids.h"

#include "../panels.h"

#include "nodeMapEditor.h"

#define INTERACTION_MODE_NONE 0
#define INTERACTION_MODE_MOVING_NODE 1
#define INTERACTION_MODE_WIRING 2

unsigned int interaction_mode = INTERACTION_MODE_NONE;

static SDL_Surface* labels[NUM_LABELS] = {0};
typedef SDL_Rect node_rect;

node_rect node_titlebars[MAX_NODES];

static int visible_connections = 0;

#define CONNECTION_INPUT 0 
#define CONNECTION_OUTPUT 1

typedef struct {
	node_rect location;
	mapper_node *owner;
	unsigned int type;
	unsigned int index;
} connection_mouse_target;


connection_mouse_target* wiring_from_connection;

connection_mouse_target connection_mouse_targets[MAX_NODES*MAX_NODE_CHANNELS*2];
node_rect connection_locations[MAX_NODES*MAX_NODE_CHANNELS*2];


void add_input_mouse_target(SDL_Rect* r, mapper_node *t, unsigned int con_type, unsigned int index) {
	connection_mouse_targets[visible_connections].owner = t;
	connection_mouse_targets[visible_connections].type = con_type;
	connection_mouse_targets[visible_connections].index = index;
	connection_mouse_targets[visible_connections].location = *r;
	visible_connections++;
}

void initNodeMapEditor(){
		//node_resource_ids.h
		LOAD_NODE_IMAGE_RESOURCES
}

void destroyNodeMapEditor(){
	int i;
	for(i=0; i< NUM_LABELS; ++i) {
		if(labels[i] != 0)
				SDL_FreeSurface(labels[i]);
	}
}

#define MAPPER_SIZE 150
int get_node_gui_height(mapper_node* node) {
	if(node->node_label == LABEL_MAPPER ) {
			return MAPPER_SIZE;
	}
	return 0;
}
int get_node_gui_width(mapper_node* node) {
	if(node->node_label == LABEL_MAPPER ) {
			return MAPPER_SIZE;
	}
	return 150;
}

int usable_input_channels(mapper_node* node) {
	if( node->binding_mode != BINDING_MODE_INPUT_DRIVER ) 
			return node->input_channels; else
					return 0;
}

int usable_output_channels(mapper_node* node) {
	if( node->binding_mode != BINDING_MODE_OUTPUT_DRIVER ) 
			return node->output_channels; else
					return 0;
}

void plot(SDL_Surface* target, int x, int y) {
		unsigned int coord = y*(target->w)+x;
		if(coord < (target->w*target->h)) {
			unsigned int* target_pixels = (unsigned int*)target->pixels;
			target_pixels[coord]=SDL_MapRGB(target->format,255,255,255);
		}
}

void draw_line(SDL_Surface *target, int x0,int y0,int x1,int y1) {
		int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
		int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
		int err = (dx>dy ? dx : -dy)/2, e2;

				for(;;) {
						plot(target, x0,y0);
								if (x0==x1 && y0==y1) break;
						e2 = err;
						if (e2 >-dx) { err -= dy; x0 += sx; }
						if (e2 < dy) { err += dx; y0 += sy; }
				}
}

void renderNodeMapEditor(SDL_Surface* target, UI_AREA* area){
		unsigned int channel_input_color = SDL_MapRGB(target->format,
						40,40,40);
		unsigned int channel_output_color = SDL_MapRGB(target->format,
						99,99,99);
		unsigned int bg_color = SDL_MapRGB(target->format,
						16,16,16);
		unsigned int title_color = SDL_MapRGB(target->format,
						72,72,72);
		unsigned int node_color = SDL_MapRGB(target->format,
						64,64,64);

		mapper_node** nodes = nodemapper_get_node_array();

		visible_connections = 0;

		if(interaction_mode == INTERACTION_MODE_WIRING )
				bg_color = SDL_MapRGB(target->format, 255,0,0);

		SDL_FillRect(target, (SDL_Rect*) area, bg_color);

		/** render node bodies **/
		{
			int i;
			for(i=0; i<MAX_NODES; ++i) 
			if(nodes[i]==0) break; else	
			{
				const int channel_height = 16;
				const int node_titlebar_height = 16;
				int gui_height = get_node_gui_height(nodes[i]);
				int gui_width = get_node_gui_width(nodes[i]);
				SDL_Rect r;
				int input_channels = usable_input_channels(nodes[i]);
				int output_channels = usable_output_channels(nodes[i]);

				r.x = nodes[i]->x+area->x;
				r.y = nodes[i]->y+area->y;
				r.w = gui_width;
				r.h =(  input_channels + output_channels ) * channel_height;
				r.h = r.h < gui_height ? gui_height : r.h;

				/** titlebar **/
				SDL_FillRect(target, &r, node_color);
				{	
					SDL_Rect titlebar = r;

					titlebar.h = node_titlebar_height;
					titlebar.y -= titlebar.h;
					SDL_FillRect(target, &titlebar, title_color);
					node_titlebars[i] = titlebar;

					SDL_BlitSurface( labels[ nodes[i]->node_label ], NULL, target, &titlebar );
				}
				/** channel input/outputs **/
				{
						const int label_w = 100;
						const int label_h = 16;
						const int channel_pin_size = 12;
						const int channel_border = (channel_height-channel_pin_size) / 2;
						int j;

						for(j=0; j< input_channels;++j) {
								SDL_Rect chan_rect;
								chan_rect.x = r.x + ( channel_border  );
								chan_rect.y = r.y + ( channel_height*j ) + ( channel_border );
								chan_rect.w = channel_pin_size;
								chan_rect.h = channel_pin_size;
								SDL_FillRect(target, &chan_rect, channel_input_color );

								add_input_mouse_target(&chan_rect,
														nodes[i],
														CONNECTION_INPUT,
														j);
								connection_locations[(i*MAX_NODE_CHANNELS*2)+j] = chan_rect;


								chan_rect.x += channel_pin_size;
								chan_rect.y -= channel_border;
								chan_rect.w = label_w;
								SDL_BlitSurface( labels[ nodes[i]->input_labels[j] ],
												NULL, target, &chan_rect );
						}

						for(j=0; j< output_channels;++j) {
								int label_w;
								SDL_Rect chan_rect;
								chan_rect.x = (r.x + r.w) - channel_height + channel_border;
								chan_rect.y = (channel_height * input_channels) + r.y + ( channel_height*j ) + (channel_border);

								chan_rect.w = channel_pin_size;
								chan_rect.h = channel_pin_size;
								SDL_FillRect(target, &chan_rect, channel_output_color );

								add_input_mouse_target(&chan_rect,
														nodes[i],
														CONNECTION_OUTPUT,
														j);
								connection_locations[(i*MAX_NODE_CHANNELS*2)+MAX_NODE_CHANNELS+j] = chan_rect;

								label_w = labels[ nodes[i]->output_labels[j] ]->w;
								chan_rect.x -= channel_pin_size + label_w;
								chan_rect.y -= channel_border;
								chan_rect.w = label_w;
								SDL_BlitSurface( labels[ nodes[i]->output_labels[j] ],
												NULL, target, &chan_rect );

						}
				}
			}
		}
		/** render node wires **/
		{
			int i;
			SDL_LockSurface(target);
			for(i=0; i< MAX_NODES; ++i) 
				if(nodes[i]==0) break;else {
						int j;
						for(j=0; j<nodes[i]->input_channels; ++j){
							if( nodes[i]->inputs[j] != 0 ) {
									mapper_node* foreign_node = nodes[i]->inputs[j];

									node_rect* chan_in = 
											&connection_locations[(i*MAX_NODE_CHANNELS*2)+j];

									node_rect* chan_out = 
											&connection_locations[(foreign_node->id*MAX_NODE_CHANNELS*2)+ 
											                      nodes[i]->foreign_channels[j]+ 
																  MAX_NODE_CHANNELS ];


									draw_line(target,chan_in->x,chan_in->y,chan_out->x+chan_out->w,chan_out->y+chan_out->h);
							}
						}
			}
			
			if(interaction_mode == INTERACTION_MODE_WIRING) {
					draw_line(target, 
									wiring_from_connection->location.x,
									wiring_from_connection->location.y,
									client_get_screen_mousex(),
									client_get_screen_mousey() );
			}

			SDL_UnlockSurface(target);
		}
}


unsigned int point_in_rect(int x, int y, node_rect *r) {
		if( (x>r->x) &&
			(y>r->y) &&
			(x< (r->x+r->w) ) &&
			(y< (r->y+r->h) ) ) {
			return 1;
		}
		return 0;
}

unsigned int dragging_node = 0;

unsigned int drag_origin_x = 0;
unsigned int drag_origin_y = 0;


void wire_inputs( connection_mouse_target* a, connection_mouse_target* b) {
	if(a->type == CONNECTION_OUTPUT ) {
		connection_mouse_target* tmp;
		tmp = a;
		a = b;
		b= tmp;
	}

	a->owner->inputs[ a->index ] = b->owner;
	a->owner->foreign_channels[ a->index ] = b->index;
}

void nodemapeditor_mousedown(int x,int y){
		unsigned int cmx = client_get_screen_mousex();
		unsigned int cmy = client_get_screen_mousey();
		mapper_node** nodes = nodemapper_get_node_array();

		if( interaction_mode == INTERACTION_MODE_NONE) {
				int i;
				for(i=0; i<MAX_NODES; ++i) {
						if(nodes[i] == 0) {
							break;
						}				
						if(point_in_rect( cmx,cmy, &node_titlebars[i]) == 1) {
							interaction_mode = INTERACTION_MODE_MOVING_NODE;
							dragging_node = i;
							drag_origin_x = cmx;
							drag_origin_y = cmy;
							break;
						}
				}

				for(i=0; i<visible_connections; ++i) {
					if(point_in_rect( cmx,cmy, 
									&connection_mouse_targets[i].location )) {

							interaction_mode = INTERACTION_MODE_WIRING;
							wiring_from_connection = &connection_mouse_targets[i];
							if(wiring_from_connection->type == CONNECTION_INPUT) {
								wiring_from_connection->owner->inputs[ wiring_from_connection->index ] = 0;
							}
					}
				}
		}
}

void nodemapeditor_mouseup(int x,int y){
		unsigned int cmx = client_get_screen_mousex();
		unsigned int cmy = client_get_screen_mousey();

		if(interaction_mode == INTERACTION_MODE_WIRING) {
				int i;
				for(i=0; i<visible_connections; ++i) {
						if(point_in_rect( cmx,cmy, 
												&connection_mouse_targets[i].location )) {
								connection_mouse_target *wiring_to_connection;
								wiring_to_connection = &connection_mouse_targets[i];

								// outputs and inputs must be paired
								if( wiring_to_connection->type 
												!= wiring_from_connection->type ) {
										wire_inputs( wiring_to_connection,
														wiring_from_connection );
								}
						}
				}

		}

		interaction_mode = INTERACTION_MODE_NONE;
}

void nodemapeditor_mousemotion(int x,int y){
		unsigned int cmx = client_get_screen_mousex();
		unsigned int cmy = client_get_screen_mousey();
		mapper_node** nodes = nodemapper_get_node_array();

		if( interaction_mode == INTERACTION_MODE_MOVING_NODE ) {

			nodes[dragging_node]->x -= (drag_origin_x - cmx);
			nodes[dragging_node]->y -= (drag_origin_y - cmy);

			drag_origin_x = cmx;
			drag_origin_y = cmy;
		}
}

void nodemapeditor_mouseleave(){
		interaction_mode = INTERACTION_MODE_NONE;
}
