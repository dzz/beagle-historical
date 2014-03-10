#include "node_mapper.h"

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

void node_no_recalc(mapper_node* node) { }

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
	node->binding_mode = BINDING_MODE_USER;
}

/*******************/

mapper_node** node_array[MAX_NODES] = {0};
mapper_node output_node;
mapper_node stylus_input;

void node_mapper_apply_input( double pressure, double time ) {
	stylus_input.outputs[0] = pressure;
	stylus_input.outputs[1] = time / 1000;
	stylus_input.outputs[2] = 0;

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
		output_node.node_label = LABEL_BRUSH_CONTROLLER;
		output_node.input_labels[0] = LABEL_SIZE;
		output_node.input_labels[1] = LABEL_COLOR;
		output_node.input_labels[2] = LABEL_ALPHA;
		output_node.input_labels[3] = LABEL_JITTER;
		output_node.input_labels[4] = LABEL_NOISE;

		for(i=0; i< output_node.input_channels; ++i) {
			output_node.inputs[i] = &stylus_input;
		}

		output_node.x = 350;
		output_node.y = 400;

		output_node.id = NODE_ID_BRUSH_CONTROLLER;

		output_node.binding_mode = BINDING_MODE_OUTPUT_DRIVER;
}

void create_stylus_input() {
		new_node(&stylus_input);
		stylus_input.output_channels = 3;

		stylus_input.node_label = LABEL_STYLUS;
		stylus_input.output_labels[0] = LABEL_PRESSURE;
		stylus_input.output_labels[1] = LABEL_TIME;
		
		stylus_input.x = 20;
		stylus_input.y = 20;

		stylus_input.id = NODE_ID_STYLUS_INPUT;
		stylus_input.binding_mode = BINDING_MODE_INPUT_DRIVER;
}

void initNodeMapper() {
		create_stylus_input();
		create_brush_controller();
		node_array[NODE_ID_STYLUS_INPUT] = &stylus_input;
		node_array[NODE_ID_BRUSH_CONTROLLER] = &output_node;
}

mapper_node** nodemapper_get_node_array() {
	return (mapper_node**)node_array;
}
