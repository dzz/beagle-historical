#include <SDL.h>
#include <math.h>
#include <malloc.h>

#include "../../system/ctt2.h"

#include "../../drawing/node_mapper.h"
#include "../../drawing/node_resource_ids.h"

#include "../panels.h"

#include "mapperEditorBank.h"

#include "nodeMapEditor.h"

#include "node_interaction/node_interaction.h"

#define INTERACTION_MODE_NONE 0
#define INTERACTION_MODE_MOVING_NODE 1
#define INTERACTION_MODE_WIRING 2

#define NO_NODE_FOCUSED -1
#define NO_NODE_TOUCHED -1
unsigned int interaction_mode = INTERACTION_MODE_NONE;
unsigned int focused_node = NO_NODE_FOCUSED;
unsigned int last_touched_node = NO_NODE_TOUCHED;

static SDL_Surface* labels[NUM_LABELS] = {0};

node_rect node_titlebars[MAX_NODES];
node_rect node_guiareas[MAX_NODES];

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

		initNodeDispatcher();
}

void destroyNodeMapEditor(){
	int i;
	for(i=0; i< NUM_LABELS; ++i) {
		if(labels[i] != 0)
				SDL_FreeSurface(labels[i]);
	}
}

int get_node_gui_height(mapper_node* node) {
		return node->gui_height;
}
#define MIN_SIZE 200
int get_node_gui_width(mapper_node* node) {
		return (node->gui_width < MIN_SIZE ) ? MIN_SIZE : node->gui_width;
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
			target_pixels[coord]=SDL_MapRGB(target->format,128,255,128);
		}
}



void draw_line(SDL_Surface *target, int x0,int y0,int x1,int y1) {
		int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
		int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
		int err = (dx>dy ? dx : -dy)/2, e2;
		int space = y1;

				for(;;) {
						space++;
						if(space%3==2) plot(target, x0,y0);
								if (x0==x1 && y0==y1) break;
						e2 = err;
						if (e2 >-dx) { err -= dy; x0 += sx; }
						if (e2 < dy) { err += dx; y0 += sy; }
				}
}

#define BEGIN_INTERFACES {
#define RENDERABLE_INTERFACE(x) if(node->interface_type==NODE_INTERFACE_##x) render_interface_##x( target, node, r ); else
#define END_INTERFACES { /* invalid interface specified */} }

void render_node_gui( SDL_Surface* target, mapper_node* node, node_rect* r) {
			BEGIN_INTERFACES
			RENDERABLE_INTERFACE(MAPPER)
			RENDERABLE_INTERFACE(COLOR)
			RENDERABLE_INTERFACE(GRADIENT)
			END_INTERFACES
}


void renderAddMenu(SDL_Surface* target, UI_AREA* area);

void renderNodeMapEditor(SDL_Surface* target, UI_AREA* area){
		unsigned int channel_input_color = SDL_MapRGB(target->format,
						40,40,40);
		unsigned int channel_output_color = SDL_MapRGB(target->format,
						99,99,99);
		unsigned int bg_color = SDL_MapRGB(target->format,
						16,16,16);
		unsigned int title_color = SDL_MapRGB(target->format,
						79,79,79);
		unsigned int node_color = SDL_MapRGB(target->format,
						64,64,64);
		unsigned int focused_node_color = SDL_MapRGB(target->format,
						164,164,164);
		unsigned int focused_title_color = SDL_MapRGB(target->format,
						82,82,82);

		mapper_node** nodes = nodemapper_get_node_array();

		visible_connections = 0;


	
		SDL_FillRect(target, (SDL_Rect*) area, bg_color);
		renderAddMenu(target,area);

		/** render node bodies **/
		{
			int i;
			for(i=0; i<MAX_NODES; ++i) 
			if(nodes[i]==0) break; else	
			{
				const int channel_height = 14;
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

				if( i == last_touched_node)
						SDL_FillRect(target, &r, focused_node_color);
				else
						SDL_FillRect(target, &r, node_color);

				/** titlebar **/
				{	
					SDL_Rect titlebar = r;

					titlebar.h = node_titlebar_height;
					titlebar.y -= titlebar.h;
					if( i == last_touched_node) SDL_FillRect(target, &titlebar, focused_title_color);
					else SDL_FillRect(target, &titlebar, title_color);
					node_titlebars[i] = titlebar;

					SDL_BlitSurface( labels[ nodes[i]->node_label ], NULL, target, &titlebar );
				}
				/** channel input/outputs **/
				{
						const int label_w = 75;
						const int label_h = 14;
						const int channel_pin_size = 12;
						const int channel_border = (channel_height-channel_pin_size) / 2;
						int j;
						int k = 0;
						
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

								k++; //see gui rendering after next for block
						}

						for(j=0; j< output_channels;++j) {
								int label_w;
								SDL_Rect chan_rect;
								chan_rect.x = (r.x + r.w) - channel_height + channel_border;
								chan_rect.y = /*(channel_height * input_channels) + */r.y + ( channel_height*j ) + (channel_border);

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

								//no increment of k here, we check max in next block
						}

						{
							node_rect gui_location;
							k = j>k ? j : k;
							gui_location.x=r.x;
							gui_location.y=r.y + (channel_height*k);
							gui_location.w=gui_width;
							gui_location.h=gui_height;
							render_node_gui( target, nodes[i], &gui_location );
							node_guiareas[i] = gui_location;
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


#define MOUSE_IN_TARGET 1

#define MENU_ITEMS 6
#define MENU_ITEM_HEIGHT 24

unsigned int button_width = 0;

void renderAddMenu(SDL_Surface* target, UI_AREA* area) {
	SDL_Rect r;
	SDL_Surface* buttons[MENU_ITEMS];
	int i;

	buttons[0] = labels[LABEL_MAPPER];
	buttons[1] = labels[LABEL_ADD];
	buttons[2] = labels[LABEL_MUL];
	buttons[3] = labels[LABEL_COLOR];
	buttons[4] = labels[LABEL_GRADIENT];
	buttons[5] = labels[LABEL_ADD3];

	r.x = area->x; 
	r.y = area->y;
	r.w = buttons[0]->w;
	r.h = buttons[0]->h;

	button_width = r.w;

	for(i=0; i<MENU_ITEMS;++i) {
		SDL_BlitSurface(buttons[i],NULL,target,&r);
		r.y+=MENU_ITEM_HEIGHT;
	}
}

void process_menu_item(unsigned int item) {
		switch(item) {
				case 0:
						nodemapper_create_template(TEMPLATE_MAPPER);
						break;
				case 1:
						nodemapper_create_template(TEMPLATE_ADD);
						break;
				case 2:
						nodemapper_create_template(TEMPLATE_MUL);
						break;
				case 3: 
						nodemapper_create_template(TEMPLATE_COLOR);
						break;
				case 4: 
						nodemapper_create_template(TEMPLATE_GRADIENT);
						break;
				case 5: 
						nodemapper_create_template(TEMPLATE_ADD3);
						break;
		}
}

void nodemapeditor_mousedown(int x,int y){

		unsigned int cmx = client_get_screen_mousex();
		unsigned int cmy = client_get_screen_mousey();
		mapper_node** nodes = nodemapper_get_node_array();

		if( interaction_mode == INTERACTION_MODE_NONE) {
				int i;

				if( x < button_width &&
					y < (MENU_ITEMS * MENU_ITEM_HEIGHT) ) {
						unsigned int item = y/ MENU_ITEM_HEIGHT;
						process_menu_item(item);
				} else {
					nodemapper_set_template_coords(x, y);
				}
				
				for(i=0; i<MAX_NODES; ++i) {
						if(nodes[i] == 0) {
							break;
						}				
						if(point_in_rect( cmx,cmy, &node_titlebars[i]) == MOUSE_IN_TARGET ) {
							interaction_mode = INTERACTION_MODE_MOVING_NODE;
							dragging_node = i;
							last_touched_node = i;
							drag_origin_x = cmx;
							drag_origin_y = cmy;
							break;
						}
						if( point_in_rect(cmx,cmy, &node_guiareas[i]) == MOUSE_IN_TARGET ) {
							focused_node = i;
							last_touched_node = i;
							node_mousedown(nodes[i],cmx,cmy);
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

		if(focused_node != -1) {
			mapper_node** nodes = nodemapper_get_node_array();
			node_mouseup(nodes[focused_node]);
			focused_node = -1;
		}
		
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

		if( focused_node != -1) {
			node_mousemotion(nodes[focused_node],cmx,cmy);
			return;
		}

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
