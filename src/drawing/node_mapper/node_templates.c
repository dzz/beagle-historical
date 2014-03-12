#include "../../user/editors/mapperEditorBank.h"
#include "../node_resource_ids.h"
#include "../node_mapper.h"
#include "node_internals.h"
#include "node_recalc.h"

void template_MAPPER(mapper_node* node) {

		new_node(node);
		node->recalc=&node_recalc_mapper;
		node->input_channels = 1;
		node->output_channels = 1;
		node->input_labels[0] = LABEL_X;
		node->output_labels[0] = LABEL_Y;
		node->node_label = LABEL_MAPPER;
		node->data = malloc(sizeof(mapping_function));

		{
				mapping_function* mf = node->data;
				mf->min_x = 0;
				mf->max_x = 1;
				mf->min_y = 0;
				mf->max_y = 1;
		}
}

#define BEGIN_TEMPLATES {
#define ADD_TEMPLATE(id) if( node_template == LABEL_##id ) template_##id(node); else
#define END_TEMPLATES { /* invalid template specified */} }

void nodemapper_create_template(unsigned int node_template) {

			mapper_node* node = nodemapper_allocate_top();
			if( node!= NO_FREE_NODES_AVAILABLE ) {
			
				BEGIN_TEMPLATES
				ADD_TEMPLATE(MAPPER)
				END_TEMPLATES

			}
			nodemapper_get_node_array()[nodemapper_get_top_id()] = node;
}

