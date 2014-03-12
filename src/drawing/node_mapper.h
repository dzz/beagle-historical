#ifndef __NODE_MAPPER__
#define __NODE_MAPPER__

#include "node_resource_ids.h"

#define MAX_NODES 32
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
		void (*update_cascade)(mapper_node* node);
		void (*recalc)(mapper_node *node);
		double outputs[MAX_NODE_CHANNELS];
		int input_channels;
		int output_channels;
		unsigned int node_label;
		unsigned int input_labels[MAX_NODE_CHANNELS];
		unsigned int output_labels[MAX_NODE_CHANNELS];
		unsigned int calculation_status;
		unsigned int binding_mode;
		//display
		unsigned int x;
		unsigned int y;
		void* data;
};

void initNodeMapper(void);
void dropNodeMapper(void);

#define BRUSH_CHANNEL_SIZE 0
#define BRUSH_CHANNEL_COLOR 1
#define BRUSH_CHANNEL_ALPHA 2
#define BRUSH_CHANNEL_JITTER 3
#define BRUSH_CHANNEL_NOISE 4

mapper_node* nodemapper_get_brush_controller();
mapper_node** nodemapper_get_node_array();

void nodemapper_create_template(unsigned int node_template);

#endif
