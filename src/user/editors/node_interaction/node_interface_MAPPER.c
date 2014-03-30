#include <math.h>
#include <SDL.h>
#include "../../../drawing/node_mapper.h"
#include "../../../drawing/node_mapper/util_curve.h"
#include "../../dispatch.h"
#include "../mapperEditorBank.h"
#include "../brushEditor.h"
#include "node_interaction.h"

void render_interface_MAPPER(SDL_Surface* target, mapper_node* node, node_rect* r ) {
		unsigned int mapper_bg = SDL_MapRGB(target->format,
						200,200,200 );
		unsigned int mapper_fg = SDL_MapRGB(target->format,
						20,20,20 );
		curve* c = (curve*)node->data;

		SDL_FillRect( target, r, mapper_bg);

		{
				int i;
				for( i=0; i<r->w;i+=3) {
					double _sP = (double)i/(double)r->w;
					double _sY = curve_compute_y_at(c,_sP);
					SDL_Rect plot;
					plot.x = i+r->x;
					plot.y = r->y+(int)( (1.0-_sY) * (double)r->h);
					plot.w=2;
					plot.h=2;
					SDL_FillRect( target, &plot, mapper_fg);
				}
		}

		node->gui_width = r->w;
		node->gui_height = r->h;
}

_cp* editing_point = 0;

#define MOUSEMODE_NONE -1
#define MOUSEMODE_DRAGGING 1

unsigned int MAPPER_MOUSEMODE = MOUSEMODE_NONE;

void node_mousemotion_MAPPER(mapper_node* node, int cmx,int cmy){
	if(MAPPER_MOUSEMODE == MOUSEMODE_DRAGGING) {
		double _sX = (double)(cmx-node->x) / (double)node->gui_width;
		double _sY = (double)(cmy-node->y) / (double)node->gui_height;
		curve* c = (curve*)node->data;
		
        if(_sX<0) _sX = 0;
        if(_sY<0) _sY = 0;
        if(_sX>1) _sX = 1;
        if(_sY>1) _sY = 1;

		if( (editing_point != &c->data[CURVE_SPECIAL_POINT_START]) &&
			(editing_point != &c->data[CURVE_SPECIAL_POINT_END]) ){
					editing_point->x = _sX;
		}
		
		editing_point->y = 1 - _sY;
	}
}

void node_mouseup_MAPPER(mapper_node* node){
	MAPPER_MOUSEMODE = MOUSEMODE_NONE;
}

void node_mousedown_MAPPER(mapper_node* node, int cmx, int cmy) {
	int i;
	double _sX = (double)(cmx-node->x) / (double)node->gui_width;
	double _sY = (double)(cmy-node->y) / (double)node->gui_height;
  
	curve *c = (curve*) node->data;

	if(  dispatch_get_modifiers()->LEFT_CONTROL == MODIFIER_ENABLED ) {
		curve_add_point(c,_sX, 1 - _sY);			
		return;
	}

	{
			const double epsilon = 1.0 / 2.0;

			double nD = fabs(_sX - c->data[0].x);
			_cp* nearest = &c->data[0];

			for(i=1; i< c->_stack_top; ++i) {
					double d;
					d = fabs(_sX - c->data[i].x);
					if(d < nD ){
							nearest = &c->data[i];
							nD = d;
					}
			}

			if( nD < epsilon ) {
							if( dispatch_get_modifiers()->LEFT_ALT == MODIFIER_ENABLED) {
								if( (nearest != &c->data[CURVE_SPECIAL_POINT_START]) &&
									(nearest != &c->data[CURVE_SPECIAL_POINT_END]) ) 
								curve_del_point(c, nearest);
							} else{
								MAPPER_MOUSEMODE = MOUSEMODE_DRAGGING;
								editing_point = nearest;
							}
			}
	}
}
