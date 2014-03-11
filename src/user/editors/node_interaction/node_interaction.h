#ifndef __NODE_INTERACTION__
#define __NODE_INTERACTION__

#include "../../../drawing/node_mapper.h"

void node_mousedown(mapper_node* node, int cmx, int cmy);
void node_mouseup(mapper_node* node);
void node_mousemotion(mapper_node* node, int cmx,int cmy);

#endif

