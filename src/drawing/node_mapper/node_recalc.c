#include "../../user/editors/mapperEditorBank.h"
#include "../node_mapper.h"
#include "node_recalc.h"

void node_no_recalc(mapper_node* node) { }

void node_passthrough(mapper_node* node) {
	int i;
	for(i=0; i<node->output_channels;++i) {
		if(node->inputs[i]!=0) {
		node->outputs[i] = node->inputs[i]->
				outputs[node->foreign_channels[i]];
		} else 
				node->outputs[i] = 0;
	}
}

void node_recalc_mapper(mapper_node* node) {
	if(node->inputs[0] != 0) {

		node->outputs[0] =
			   	mapperbank_compute_mapping( node->data, 
								node->inputs[0]-> outputs[ node->foreign_channels[0] ] );	

	}
}

void node_recalc_add(mapper_node* node) { 

}

