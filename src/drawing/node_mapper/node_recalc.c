#include "../../colors/colors.h"
#include "../../user/editors/mapperEditorBank.h"
#include "../node_mapper.h"
#include "util_gradient.h"
#include "util_curve.h"
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
			curve_compute_y_at(node->data, 
					node->inputs[0]-> outputs[ node->foreign_channels[0] ] );	

	}
}

void node_recalc_add(mapper_node* node) { 
	int i;
	node->outputs[0] = 0;	
	for(i=0; i< node->input_channels; ++i) {
		if(node->inputs[i]!=0) {
			node->outputs[0]+=
				node->inputs[i]->outputs[ node->foreign_channels[i]];
		}
	}
}

void node_recalc_add3(mapper_node* node) { 
	int i;
	for(i=0; i< node->input_channels; ++i) {
		if(node->inputs[i] ==0 )
			return;
	}
	for(i=0; i<3; ++i) {
		node->outputs[i] = 
			node->inputs[i]->outputs[ node->foreign_channels[i] ] +
			node->inputs[i+3]->outputs[ node->foreign_channels[i+3] ];
	}
}

void node_recalc_mul(mapper_node* node) { 
	int i;
	node->outputs[0] = 1;	
	for(i=0; i< node->input_channels; ++i) {
		node->outputs[0]*=node->inputs[0]->outputs[ node->foreign_channels[0]];
	}
}

void node_recalc_color(mapper_node* node) {
	cp_color* c = (cp_color*)node->data;

	node->outputs[0] = (double)c->r / 255.0;
	node->outputs[1] = (double)c->g / 255.0;
	node->outputs[2] = (double)c->b / 255.0;

}

void node_recalc_gradient(mapper_node* node) {

	gradient* g = (gradient*)node->data;

	if(node->inputs[0]!=0) {
		cp_color c = {0};

		printf("addr of c:%d\n",&c);
		c = gradient_compute_color_at( g, 
				node->inputs[0]->
				outputs[ node->foreign_channels[0 
				]]
				);

		printf("addr of c:%d\n",&c);

		node->outputs[0] = (double)(c.r)/255.0;
		node->outputs[1] = (double)(c.g)/255.0;
		node->outputs[2] = (double)(c.b)/255.0;
	}
}
