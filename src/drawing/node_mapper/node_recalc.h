#ifndef __NODE_RECALC__
#define __NODE_RECALC__

#include "../node_mapper.h"

void node_no_recalc(mapper_node* node);
void node_passthrough(mapper_node* node);
void node_recalc_mapper(mapper_node* node);
void node_recalc_add(mapper_node* node);
void node_recalc_mul(mapper_node* node);
void node_recalc_color(mapper_node* node);
void node_recalc_gradient(mapper_node* node);

#endif
