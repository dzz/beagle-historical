#include "../node_mapper.h"
#include "node_internals.h"
#include "node_recalc.h"

#define STYLUS_CHANNEL_PRESSURE 0
#define STYLUS_CHANNEL_TIME 1
#define STYLUS_CHANNEL_AZIMUTH 2

void create_node_brush_interface(mapper_node** nodes) {
        /* these are preallocated*/
		mapper_node* output_node = nodes[NODE_ID_BRUSH_CONTROLLER];
		mapper_node* stylus_input = nodes[NODE_ID_STYLUS_INPUT];
        mapper_node* color_node;

        /* build one convenience color node */
        nodemapper_set_template_coords(350,350);

        color_node = nodemapper_create_template(TEMPLATE_COLOR);

        /* create the brush driver */
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

        /* bind to pressure */
		output_node->inputs[BRUSH_CHANNEL_SIZE] 
            = nodes[NODE_ID_STYLUS_INPUT];
        output_node->foreign_channels[BRUSH_CHANNEL_SIZE] = 
            STYLUS_CHANNEL_PRESSURE;

		output_node->inputs[BRUSH_CHANNEL_ALPHA] 
            = nodes[NODE_ID_STYLUS_INPUT];
        output_node->foreign_channels[BRUSH_CHANNEL_ALPHA] = 
            STYLUS_CHANNEL_PRESSURE;

        /* bind to color */
        output_node->inputs[BRUSH_CHANNEL_R] = color_node;
        output_node->foreign_channels[BRUSH_CHANNEL_R] = 0;
        output_node->inputs[BRUSH_CHANNEL_G] = color_node;
        output_node->foreign_channels[BRUSH_CHANNEL_G] = 1;
        output_node->inputs[BRUSH_CHANNEL_B] = color_node;
        output_node->foreign_channels[BRUSH_CHANNEL_B] = 2;

        /* leave unbound */
		output_node->inputs[BRUSH_CHANNEL_JITTER] = 0;
		output_node->inputs[BRUSH_CHANNEL_NOISE] = 0;

		output_node->x = 750;
		output_node->y = 400;
		output_node->id = NODE_ID_BRUSH_CONTROLLER;
		output_node->binding_mode = BINDING_MODE_OUTPUT_DRIVER;

        /* create the stylus driver */
		new_node(nodes[NODE_ID_STYLUS_INPUT]);
		stylus_input->output_channels = 3;

		stylus_input->node_label = LABEL_STYLUS;
		stylus_input->output_labels[STYLUS_CHANNEL_PRESSURE] 
            = LABEL_PRESSURE;
		stylus_input->output_labels[STYLUS_CHANNEL_TIME] 
            = LABEL_TIME;
		stylus_input->output_labels[STYLUS_CHANNEL_AZIMUTH] 
            = LABEL_AZIMUTH;
		
		stylus_input->x = 120;
		stylus_input->y = 40;

		stylus_input->id = NODE_ID_STYLUS_INPUT;
		stylus_input->binding_mode = BINDING_MODE_INPUT_DRIVER;

}

