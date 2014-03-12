#include "../node_mapper.h"
#include "node_internals.h"
#include "node_recalc.h"

void create_node_brush_interface(mapper_node** nodes) {
		int i;
		mapper_node* output_node = nodes[NODE_ID_BRUSH_CONTROLLER];
		mapper_node* stylus_input = nodes[NODE_ID_BRUSH_CONTROLLER];

		new_node(nodes[NODE_ID_BRUSH_CONTROLLER]);
		output_node->inputs[0] = stylus_input;
		output_node->recalc = &node_passthrough;
		output_node->input_channels = 5;
		output_node->output_channels = 5;
		output_node->node_label = LABEL_BRUSH_CONTROLLER;
		output_node->input_labels[0] = LABEL_SIZE;
		output_node->input_labels[1] = LABEL_COLOR;
		output_node->input_labels[2] = LABEL_ALPHA;
		output_node->input_labels[3] = LABEL_JITTER;
		output_node->input_labels[4] = LABEL_NOISE;

		for(i=0; i< output_node->input_channels; ++i) {
			output_node->inputs[i] = nodes[NODE_ID_STYLUS_INPUT];
		}

		output_node->x = 350;
		output_node->y = 400;
		output_node->id = NODE_ID_BRUSH_CONTROLLER;
		output_node->binding_mode = BINDING_MODE_OUTPUT_DRIVER;

		new_node(nodes[NODE_ID_STYLUS_INPUT]);
		stylus_input->output_channels = 2;

		stylus_input->node_label = LABEL_STYLUS;
		stylus_input->output_labels[0] = LABEL_PRESSURE;
		stylus_input->output_labels[1] = LABEL_TIME;
		
		stylus_input->x = 20;
		stylus_input->y = 20;

		stylus_input->id = NODE_ID_STYLUS_INPUT;
		stylus_input->binding_mode = BINDING_MODE_INPUT_DRIVER;
}

