#include <SDL.h>
#include "../../../drawing/node_mapper.h"
#include "../mapperEditorBank.h"
#include "../brushEditor.h"
#include "node_interaction.h"

static mapping_function* editing_function = 0;
static int editing_function_node;

void render_interface_MAPPER(SDL_Surface* target, mapper_node* node, node_rect* r ) {
		unsigned int mapper_bg = SDL_MapRGB(target->format,
						200,200,200 );
		SDL_FillRect( target, r, mapper_bg);
		render_mapping_function(target, node->data, r);		
		{
				unsigned int y = r->y+(unsigned int)(node->outputs[0]*r->h);
				SDL_Rect marker;
				marker.x=r->x;
				marker.y=y;
				marker.w=r->w;
				marker.h=2;
				SDL_FillRect( target, &marker, 0);
		}
}

void node_mousemotion_MAPPER(mapper_node* node, int cmx,int cmy) {
		int x = cmx;
		int y = cmy;

		if( editing_function != 0 ) {
				double t_x = 
						(double)(x - editing_function->_rendered_at.x)/ 
						(double)editing_function->_rendered_at.w;

				double t_y = 
						(double)(y - editing_function->_rendered_at.y)/ 
						(double)editing_function->_rendered_at.h;

				t_y = 1 - t_y;

				if( editing_function_node == MAPPER_NODE_MIN ) {
						editing_function->min_y = t_y;

						if(t_x < editing_function->max_x ){
								editing_function->min_x = t_x;
						}
				} else

				if( editing_function_node == MAPPER_NODE_MAX ) {
						editing_function->max_y = t_y;

						if(t_x > editing_function->min_x )
								editing_function->max_x = t_x;
				} 
			
		} else {
				//other handlers
		}
}

void node_mouseup_MAPPER(mapper_node* node){
	editing_function = 0;
	brusheditor_redraw_stroke();
}

void node_mousedown_MAPPER(mapper_node* node, int cmx, int cmy) {
		const int md_threshold = 400;
		int i;

		int x = cmx;
		int y = cmy;

		mapping_function* func = (mapping_function*)node->data;
		int nodes [4];


		int node_idx;

		nodes[0] = func->_min_render_x;
		nodes[1] = func->_min_render_y;
		nodes[2] = func->_max_render_x;
		nodes[3] = func->_max_render_y;

		for(node_idx=0; node_idx <2; ++node_idx) {
				int dx = x - nodes[node_idx*2];
				int dy = y - nodes[(node_idx*2)+1];
				int md = dx*dx+dy*dy;
				if( md < md_threshold ) {
						editing_function = func;
						editing_function_node = node_idx; // this will correspond to MAPPER_NODE_MIN or MAPPER_NODE_MAX
						printf("%u\n",editing_function_node);
				}
		}
}
