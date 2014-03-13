#include "../node_mapper.h"
#include "node_internals.h"
#include "../node_resource_ids.h"
#include "node_recalc.h"

void new_node(mapper_node* node) {
	int i;
	for( i=0; i < MAX_NODE_CHANNELS; ++i) {
		node->inputs[i] = 0;
		node->foreign_channels[i] = 0;
	}
	node->input_channels = 1;
	node->update_cascade=&node_update_cascade;
	node->recalc=&node_no_recalc;
	node->calculation_status = NODE_STALE;
	node->binding_mode = BINDING_MODE_USER;
	node->interface_type = NODE_INTERFACE_NONE;
	node->x = 25;
	node->y = 25;
}

