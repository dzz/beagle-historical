#include <SDL.h>
#include "../../../drawing/node_mapper.h"
#include "../../../colors/colors.h"
#include "../colorPicker.h"
#include "node_interaction.h"

void render_interface_GRADIENT( SDL_Surface* target, mapper_node* node, node_rect* r ) {
		SDL_FillRect(target, r, SDL_MapRGB(target->format, 255,255,255));
}

void node_mousemotion_GRADIENT(mapper_node* node, int cmx,int cmy){}
void node_mouseup_GRADIENT(mapper_node* node){}

void node_mousedown_GRADIENT(mapper_node* node, int cmx, int cmy) {
}
