#include "../node_mapper.h"
#include "node_internals.h"

#define NODE_TOUCHED 1

void _node_stale_cascade(mapper_node* node, unsigned int* touched) {
		int i;
		touched[node->id] = NODE_TOUCHED;
		for( i=0; i < node->input_channels; ++i) {
				if(node->inputs[i] != 0) {
						if( touched[node->inputs[i]->id] != NODE_TOUCHED ){
								_node_stale_cascade( node->inputs[i], touched );
						}
				}
		}	
		node->calculation_status = NODE_STALE;
}



void _node_update_cascade(mapper_node* node, unsigned int* touched) {
		int i;
		touched[node->id] = NODE_TOUCHED;
		if( node-> calculation_status == NODE_STALE) {
				for( i=0; i < node->input_channels; ++i) {
						if(node->inputs[i] != 0) {
								if( touched[node->inputs[i]->id] != NODE_TOUCHED ){
										_node_update_cascade(node->inputs[i], touched);
								}
						}
				}	
				node->recalc(node);
				node->calculation_status = NODE_FRESH;
		}
}

void node_update_cascade(mapper_node* node) {
		unsigned int touched[MAX_NODES] = {0};
		_node_update_cascade(node, &touched);
}

void node_stale_cascade(mapper_node* node) {
		unsigned int touched[MAX_NODES] = {0};
		_node_stale_cascade(node, &touched);
}
