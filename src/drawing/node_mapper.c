#include <malloc.h>

#include "node_mapper.h"
#include "node_mapper/node_internals.h"

static unsigned int top_node_id = 0;

/*******************/

mapper_node** node_array[MAX_NODES] = {0};
mapper_node output_node;
mapper_node stylus_input;

void node_mapper_apply_input( double pressure, double time, double azimuth, 
        double orientation ) {
	stylus_input.outputs[0] = pressure;
	stylus_input.outputs[1] = time / 1000.0;
	stylus_input.outputs[2] = azimuth;
    stylus_input.outputs[3] = orientation;
	node_stale_cascade( &output_node );
}

mapper_node* nodemapper_get_brush_controller() {
		node_update_cascade(&output_node);
		return &output_node;
}

void initNodeMapper() {
		node_array[NODE_ID_STYLUS_INPUT] = &stylus_input;
		node_array[NODE_ID_BRUSH_CONTROLLER] = &output_node;
		top_node_id = 1;
		create_node_brush_interface(node_array);
}

void dropNodeMapper() {
	int i;
	for(i=2; i< MAX_NODES; ++i) {
		if( node_array[i] != 0 ) {
			mapper_node* node = node_array[i];
			if(node->data != 0) {
				free(node->data);
			}
			free(node_array[i]);
		}
	}
}

mapper_node** nodemapper_get_node_array() {
	return (mapper_node**)node_array;
}

unsigned int nodemapper_get_top_id() {
	return top_node_id;
}

mapper_node* nodemapper_allocate_top(){
	if(top_node_id+1 < MAX_NODES ) {
		top_node_id++;
		{
			mapper_node* node = (mapper_node*)malloc(sizeof(mapper_node));
			node->id=top_node_id;
			new_node(node);
			return node;
		} 
		} else {
			return NO_FREE_NODES_AVAILABLE;
	}
}
