#include "../node_mapper.h"
#include "node_internals.h"
#include "node_recalc.h"

void create_node_brush_interface(mapper_node** nodes) {
		int i;
		mapper_node* output_node = nodes[NODE_ID_BRUSH_CONTROLLER];
		mapper_node* stylus_input = nodes[NODE_ID_STYLUS_INPUT];

		new_node(nodes[NODE_ID_BRUSH_CONTROLLER]);
		output_node->inputs[0] = stylus_input;
		output_node->recalc = &node_passthrough;
		output_node->input_channels = 7;
		output_node->output_channels = 7;
		output_node->node_label = LABEL_BRUSH_CONTROLLER;
		output_node->input_labels[BRUSH_CHANNEL_SIZE] = LABEL_SIZE;
		output_node->input_labels[BRUSH_CHANNEL_ALPHA] = LABEL_ALPHA;
		output_node->input_labels[BRUSH_CHANNEL_JITTER] = LABEL_JITTER;
		output_node->input_labels[BRUSH_CHANNEL_NOISE] = LABEL_NOISE;
		output_node->input_labels[BRUSH_CHANNEL_R] = LABEL_R;
		output_node->input_labels[BRUSH_CHANNEL_G] = LABEL_G;
		output_node->input_labels[BRUSH_CHANNEL_B] = LABEL_B;

		for(i=0; i< output_node->input_channels; ++i) {
			output_node->inputs[i] = nodes[NODE_ID_STYLUS_INPUT];
		}
		output_node->inputs[BRUSH_CHANNEL_JITTER] = 0;
		output_node->inputs[BRUSH_CHANNEL_NOISE] = 0;

		output_node->x = 750;
		output_node->y = 400;
		output_node->id = NODE_ID_BRUSH_CONTROLLER;
		output_node->binding_mode = BINDING_MODE_OUTPUT_DRIVER;

		new_node(nodes[NODE_ID_STYLUS_INPUT]);
		stylus_input->output_channels = 3;

		stylus_input->node_label = LABEL_STYLUS;
		stylus_input->output_labels[0] = LABEL_PRESSURE;
		stylus_input->output_labels[1] = LABEL_TIME;
		stylus_input->output_labels[2] = LABEL_AZIMUTH;
		
		stylus_input->x = 120;
		stylus_input->y = 40;

		stylus_input->id = NODE_ID_STYLUS_INPUT;
		stylus_input->binding_mode = BINDING_MODE_INPUT_DRIVER;
}

