#include "../node_mapper.h"
#include "node_internals.h"

void node_stale_cascade(mapper_node* node) {
		int i;
		for( i=0; i < node->input_channels; ++i) {
				if(node->inputs[i] != 0) {
						node_stale_cascade( node->inputs[i] );
				}
		}	
		node->calculation_status = NODE_STALE;
}

void node_update_cascade(mapper_node* node) {
		int i;
		if( node-> calculation_status == NODE_STALE) {
				for( i=0; i < node->input_channels; ++i) {
						if(node->inputs[i] != 0) {
								node->inputs[i]->update_cascade(node->inputs[i]);
						}
				}	
				node->recalc(node);
				node->calculation_status = NODE_FRESH;
		}
}
