#ifndef __NODE_MAPPER__
#define __NODE_MAPPER__

#include "node_resource_ids.h"

#define MAX_NODES 64
#define MAX_NODE_CHANNELS 8

#define NODE_STALE 1
#define NODE_FRESH 0

typedef struct mapper_node mapper_node;

#define BINDING_MODE_USER 0
#define BINDING_MODE_INPUT_DRIVER 1
#define BINDING_MODE_OUTPUT_DRIVER 2

#define NODE_ID_STYLUS_INPUT 0
#define NODE_ID_BRUSH_CONTROLLER 1

struct mapper_node{
		//data
		unsigned int id;
		mapper_node* inputs[MAX_NODE_CHANNELS];
		int foreign_channels[MAX_NODE_CHANNELS];
		void (*recalc)(mapper_node *node);
		double outputs[MAX_NODE_CHANNELS];
		int input_channels;
		int output_channels;
		unsigned int node_label;
		unsigned int interface_type;
		unsigned int input_labels[MAX_NODE_CHANNELS];
		unsigned int output_labels[MAX_NODE_CHANNELS];
		unsigned int calculation_status;
		unsigned int binding_mode;
		//display
		unsigned int x;
		unsigned int y;
		unsigned int gui_width;
		unsigned int gui_height;
		void* data;
};

void initNodeMapper(void);
void dropNodeMapper(void);

#define BRUSH_CHANNEL_SIZE 0
#define BRUSH_CHANNEL_ALPHA 1
#define BRUSH_CHANNEL_JITTER 2
#define BRUSH_CHANNEL_NOISE 3
#define BRUSH_CHANNEL_R 4
#define BRUSH_CHANNEL_G 5
#define BRUSH_CHANNEL_B 6

mapper_node* nodemapper_get_brush_controller();
mapper_node** nodemapper_get_node_array();

void nodemapper_create_template(unsigned int node_template);
void nodemapper_set_template_coords(int x, int y);
void node_mapper_apply_input( double pressure, double time, double azimuth );

#endif
