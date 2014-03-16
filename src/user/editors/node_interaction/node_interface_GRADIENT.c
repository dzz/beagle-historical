#include <SDL.h>
#include <math.h>

#include "../../../drawing/node_mapper.h"
#include "../../../drawing/node_mapper/util_gradient.h"
#include "../../../colors/colors.h"
#include "../colorPicker.h"

#include "node_interaction.h"

void render_interface_GRADIENT( SDL_Surface* target, mapper_node* node, node_rect* r ) {
		/* should not do this EVERY redraw. */
		double p = 0;
		double p_delta = 1.0/255.0;
		double scr_delta = (double)r->w/255.0;
		double x = 0;
		int i;
		SDL_Rect sliver; 

		gradient *g = (gradient*)node->data;

		sliver.w = (int)scr_delta + 1;
		sliver.y = r->y;
		sliver.h = r->h;

		SDL_FillRect(target, r, SDL_MapRGB(target->format, 255,255,255));

		for( i = 0; i<255; ++i ) {
				cp_color c = gradient_compute_color_at(g, p );
				sliver.x = (int)x+r->x;
				x+=scr_delta;
				SDL_FillRect(target,&sliver, SDL_MapRGB( target->format, c.r,c.g,c.b ));
				p+=p_delta;
		}
}

void node_mousemotion_GRADIENT(mapper_node* node, int cmx,int cmy){}
void node_mouseup_GRADIENT(mapper_node* node){}

void node_mousedown_GRADIENT(mapper_node* node, int cmx, int cmy) {
	int i;
	double _sX = (double)(cmx-node->x) / (double)node->gui_width;
	gradient *g = (gradient*) node->data;

	double nD = fabs(_sX - g->data[0].x);
	_gp* nearest = &g->data[0];

	for(i=1; i< g->_stack_top; ++i) {
			double d;
			d = fabs(_sX - g->data[i].x);
			if(d < nD ){
				nearest = &g->data[i];
				nD = d;
			}
	}
	bindColorPickerTarget(&nearest->c);
}
