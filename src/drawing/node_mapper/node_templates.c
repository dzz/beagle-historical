#include "../../colors/colors.h"
#include "../node_resource_ids.h"
#include "../node_mapper.h"
#include "node_internals.h"
#include "util_gradient.h"
#include "util_curve.h"
#include "node_recalc.h"

void template_GRADIENT(mapper_node* node) {

		node->recalc=&node_recalc_gradient;

		node->input_channels = 1;
		node->output_channels = 3;
		node->input_labels[0] = LABEL_X;
		node->output_labels[0] = LABEL_R;
		node->output_labels[1] = LABEL_G;
		node->output_labels[2] = LABEL_B;

		node->node_label = LABEL_GRADIENT;
		node->interface_type = NODE_INTERFACE_GRADIENT;
		node->gui_width= 300;
		node->gui_height =40;

		node->data = malloc(sizeof(gradient));
		init_gradient(node->data);
}

void template_COLOR(mapper_node* node) {

		node->recalc=&node_recalc_color;

		node->input_channels = 0;
		node->output_channels = 3;
		node->output_labels[0] = LABEL_R;
		node->output_labels[1] = LABEL_G;
		node->output_labels[2] = LABEL_B;

		node->node_label = LABEL_COLOR;
		node->interface_type = NODE_INTERFACE_COLOR;
		node->gui_width= 50;
		node->gui_height =25;

		node->data = malloc(sizeof(cp_color));
		{
			cp_color* col = (cp_color*)node->data;
			col->r = 32;
			col->g = 16;
			col->b = 172;
		}
}

void template_ADD(mapper_node* node) {

		node->recalc=&node_recalc_add;

		node->input_channels = 2;
		node->output_channels = 1;
		node->input_labels[0] = LABEL_X;
		node->input_labels[1] = LABEL_X;
		node->output_labels[0] = LABEL_Y;

		node->node_label = LABEL_ADD;
		node->interface_type = NODE_INTERFACE_NONE;
}

void template_ADD3(mapper_node* node) {

		node->recalc=&node_recalc_add3;

		node->input_channels = 6;
		node->output_channels = 3;
		node->input_labels[0] = LABEL_A;
		node->input_labels[1] = LABEL_A;
		node->input_labels[2] = LABEL_A;
		node->input_labels[3] = LABEL_B;
		node->input_labels[4] = LABEL_B;
		node->input_labels[5] = LABEL_B;
		node->output_labels[0] = LABEL_Y;
		node->output_labels[1] = LABEL_Y;
		node->output_labels[2] = LABEL_Y;

		node->node_label = LABEL_ADD3;
		node->interface_type = NODE_INTERFACE_NONE;
}

void template_MUL(mapper_node* node) {

		node->recalc=&node_recalc_mul;

		node->input_channels = 2;
		node->output_channels = 1;

		node->input_labels[0] = LABEL_X;
		node->input_labels[1] = LABEL_X;
		node->output_labels[0] = LABEL_Y;

		node->node_label = LABEL_MUL;
		node->interface_type = NODE_INTERFACE_NONE;
}

void template_MAPPER(mapper_node* node) {

		node->recalc=&node_recalc_mapper;

		node->input_channels = 1;
		node->output_channels = 1;
		node->input_labels[0] = LABEL_X;
		node->output_labels[0] = LABEL_Y;

		node->node_label = LABEL_MAPPER;
		node->interface_type = NODE_INTERFACE_MAPPER;
		node->gui_width = 150;
		node->gui_height = 150;

		node->data = malloc(sizeof(curve));
		init_curve( node->data);
}

/******************************/
/******************************/
/******************************/

#define BEGIN_TEMPLATES {
#define ADD_TEMPLATE(id) if( node_template == TEMPLATE_##id ) template_##id(node); else
#define END_TEMPLATES { /* invalid template specified */} }

static int template_x = 25;
static int template_y = 25;

void nodemapper_set_template_coords(int x, int y) {
		template_x = x;
		template_y = y;	
}

mapper_node* nodemapper_create_template(unsigned int node_template) {

			mapper_node* node = nodemapper_allocate_top();
			if( node!= NO_FREE_NODES_AVAILABLE ) {
			
				BEGIN_TEMPLATES
				ADD_TEMPLATE(MAPPER)
				ADD_TEMPLATE(ADD)
				ADD_TEMPLATE(ADD3)
				ADD_TEMPLATE(MUL)
				ADD_TEMPLATE(COLOR)
				ADD_TEMPLATE(GRADIENT)
				END_TEMPLATES

				node->x = template_x;
				node->y = template_y;
			}
			nodemapper_get_node_array()[nodemapper_get_top_id()] = node;
            return node;
}

