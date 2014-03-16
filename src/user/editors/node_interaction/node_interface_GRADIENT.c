#include <SDL.h>
#include <math.h>

#include "../../../drawing/node_mapper.h"
#include "../../../drawing/node_mapper/util_gradient.h"
#include "../../../colors/colors.h"
#include "../../dispatch.h"
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

		for( i=0; i<g->_stack_top; ++i) {
			SDL_Rect gr;
			gr.x = (int)(g->data[i].x*(double)r->w)+r->x;
			if(gr.x+4 >= (r->x+r->w)) gr.x = r->x+(r->w-4);
			gr.w = 2;
			gr.h = 2;
			gr.y = r->y;
			SDL_FillRect(target,&gr, SDL_MapRGB(target->format,0,0,0) );
			gr.x+=2;
			SDL_FillRect(target,&gr, SDL_MapRGB(target->format,255,255,255) );
		}
}

_gp* editing_stop = 0;


#define MOUSEMODE_NONE -1
#define MOUSEMODE_DRAGGING 1

unsigned int GRADIENT_MOUSEMODE = MOUSEMODE_NONE;

void node_mousemotion_GRADIENT(mapper_node* node, int cmx,int cmy){
	if(GRADIENT_MOUSEMODE == MOUSEMODE_DRAGGING) {
		double _sX = (double)(cmx-node->x) / (double)node->gui_width;
		editing_stop->x = _sX;
	}
}

void node_mouseup_GRADIENT(mapper_node* node){
	GRADIENT_MOUSEMODE = MOUSEMODE_NONE;
}

void node_mousedown_GRADIENT(mapper_node* node, int cmx, int cmy) {

	int i;
	double _sX = (double)(cmx-node->x) / (double)node->gui_width;
	gradient *g = (gradient*) node->data;

	if(  dispatch_get_modifiers()->LEFT_CONTROL == MODIFIER_ENABLED ) {
		gradient_add_stop(g,_sX);			
		return;
	}
	{
			const double epsilon = 1.0 / 32.0;


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
			if( nD < epsilon ) {
					if( (nearest != &g->data[0]) &&
						(nearest != &g->data[1]) ) {
							if( dispatch_get_modifiers()->LEFT_ALT == MODIFIER_ENABLED) {

								gradient_del_stop(g, nearest);

							} else{
								GRADIENT_MOUSEMODE = MOUSEMODE_DRAGGING;
								editing_stop = nearest;
							}
					}

			}
	}
}
