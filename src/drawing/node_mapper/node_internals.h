#ifndef __NODE_INTERNALS__
#define __NODE_INTERNALS__

#include "../node_mapper.h"

void create_node_brush_interface(mapper_node** nodes);

void new_node(mapper_node* node);
void nodemapper_add_node();

void node_stale_cascade(mapper_node* node);
void node_update_cascade(mapper_node* node);

unsigned int nodemapper_get_top_id();

#define NO_FREE_NODES_AVAILABLE 0
mapper_node* nodemapper_allocate_top();

#endif
