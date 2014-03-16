#include <SDL.h>
#include "../../../drawing/node_mapper.h"
#include "../../../colors/colors.h"
#include "../colorPicker.h"
#include "node_interaction.h"

void render_interface_COLOR( SDL_Surface* target, mapper_node* node, node_rect* r ) {
		cp_color* c = (cp_color*)node->data;
		SDL_FillRect( target, r, SDL_MapRGB( target->format, 
								c->r,c->g,c->b ) );
}

void node_mousemotion_COLOR(mapper_node* node, int cmx,int cmy){}
void node_mouseup_COLOR(mapper_node* node){}

void node_mousedown_COLOR(mapper_node* node, int cmx, int cmy) {
	bindColorPickerTarget( (cp_color*)node->data );		
}
