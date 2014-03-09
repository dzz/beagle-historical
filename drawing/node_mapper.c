#include "node_mapper.h"

static double p = 0;

#define MAX_NODE_CHANNELS 8

#define NODE_STALE 1
#define NODE_FRESH 0

typedef struct mapper_node mapper_node;

struct mapper_node{
		mapper_node* inputs[MAX_NODE_CHANNELS];
		void (*update_cascade)(mapper_node* node);
		void (*recalc)(mapper_node *node);
		double outputs[MAX_NODE_CHANNELS];
		int channels;
		unsigned int calculation_status;
};


void node_stale_cascade(mapper_node* node) {
		int i;
		for( i=0; i < node->channels; ++i) {
				if(node->inputs[i] != 0) {
						node_stale_cascade( node->inputs[i] );
				}
		}	
		node->calculation_status = NODE_STALE;
}

void node_update_cascade(mapper_node* node) {
		int i;
		if( node-> calculation_status == NODE_STALE) {
				for( i=0; i < node->channels; ++i) {
						if(node->inputs[i] != 0) {
								node->inputs[i]->update_cascade(node->inputs[i]);
						}
				}	
				node->recalc(node);
				node->calculation_status = NODE_FRESH;
		}
}

void node_no_recalc(mapper_node* node) {

}

void node_passthrough(mapper_node* node) {
	node->outputs[0] = node->inputs[0]->outputs[0];
}

void new_node(mapper_node* node) {
	int i;
	for( i=0; i < MAX_NODE_CHANNELS; ++i) {
		node->inputs[i] = 0;
	}
	node->channels = 1;
	node->update_cascade=&node_update_cascade;
	node->recalc=&node_no_recalc;
}


mapper_node size_binding;
mapper_node stylus_input;

void node_mapper_apply_input( double pressure ) {
	stylus_input.outputs[0] = pressure;
	node_stale_cascade( &size_binding );
}

double node_mapper_get_output_at(unsigned int address){
		node_update_cascade(&size_binding);
		return size_binding.outputs[0];
}

void initNodeMapper() {
		new_node(&stylus_input);
		new_node(&size_binding);
		size_binding.inputs[0] = &stylus_input;
		size_binding.recalc = &node_passthrough;
}
