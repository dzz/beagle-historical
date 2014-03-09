#include "node_mapper.h"

static double p = 0;





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

void node_no_recalc(mapper_node* node) {

}

void node_passthrough(mapper_node* node) {
	int i;
	for(i=0; i<node->output_channels;++i) {
		node->outputs[i] = node->inputs[i]->
				outputs[node->foreign_channels[i]];
	}
}

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
}


mapper_node output_node;
mapper_node stylus_input;

void node_mapper_apply_input( double pressure, double time ) {
	stylus_input.outputs[0] = pressure;
	stylus_input.outputs[1] = time / 1000;

	node_stale_cascade( &output_node );
}

mapper_node* nodemapper_get_brush_controller() {
		node_update_cascade(&output_node);
		return &output_node;
}


void create_brush_controller() {
		int i;

		new_node(&output_node);
		output_node.inputs[0] = &stylus_input;
		output_node.recalc = &node_passthrough;
		output_node.input_channels = 5;
		output_node.output_channels = 5;
		output_node.name = "brush_controller";
		output_node.input_names[0] = "size";
		output_node.input_names[1] = "color";
		output_node.input_names[2] = "alpha";
		output_node.input_names[3] = "jitter";
		output_node.input_names[4] = "noise";

		for(i=0; i< output_node.input_channels; ++i) {
			output_node.inputs[i] = &stylus_input;
		}
}

void create_stylus_input() {
		new_node(&stylus_input);
		stylus_input.output_channels = 2;
		stylus_input.name="stylus";
		stylus_input.output_names[0] = "pressure";
		stylus_input.output_names[1] = "time";
}

void initNodeMapper() {
		create_stylus_input();
		create_brush_controller();
}
