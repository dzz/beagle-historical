#ifndef __NODE_INTERACTION__
#define __NODE_INTERACTION__

#include <SDL.h>
#include "../../../drawing/node_mapper.h"
#include "../../../drawing/node_resource_ids.h"
#include "../nodeMapEditor.h"

void initNodeDispatcher();
void node_mousedown(mapper_node* node, int cmx, int cmy);
void node_mouseup(mapper_node* node);
void node_mousemotion(mapper_node* node, int cmx,int cmy);

#define EXPORT_INTERFACE(name)\
void node_mousedown_##name (mapper_node* node, int cmx, int cmy);\
void node_mouseup_##name (mapper_node* node);\
void node_mousemotion_##name (mapper_node* node, int cmx,int cmy);\
void render_interface_##name (SDL_Surface* target, mapper_node* node, node_rect* r );

EXPORT_INTERFACE(MAPPER)

#endif

