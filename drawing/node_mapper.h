#ifndef __NODE_MAPPER__
#define __NODE_MAPPER__

#define MAX_NODE_CHANNELS 8

#define NODE_STALE 1
#define NODE_FRESH 0

typedef struct mapper_node mapper_node;

struct mapper_node{
		mapper_node* inputs[MAX_NODE_CHANNELS];
		int foreign_channels[MAX_NODE_CHANNELS];
		void (*update_cascade)(mapper_node* node);
		void (*recalc)(mapper_node *node);
		double outputs[MAX_NODE_CHANNELS];
		int input_channels;
		int output_channels;
		char* name;
		char* input_names[MAX_NODE_CHANNELS];
		char* output_names[MAX_NODE_CHANNELS];
		unsigned int calculation_status;
};

void initNodeMapper(void);

#define BRUSH_CHANNEL_SIZE 0
#define BRUSH_CHANNEL_COLOR 1
#define BRUSH_CHANNEL_ALPHA 2
#define BRUSH_CHANNEL_JITTER 3
#define BRUSH_CHANNEL_NOISE 4

mapper_node* nodemapper_get_brush_controller();

#endif
