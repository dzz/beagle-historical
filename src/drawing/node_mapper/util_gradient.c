#include "util_gradient.h"

void init_gradient(gradient* g) {

	g->data[GRADIENT_SPECIAL_STOP_START].x = 0;
	g->data[GRADIENT_SPECIAL_STOP_START].c.r=0;
	g->data[GRADIENT_SPECIAL_STOP_START].c.g=0;
	g->data[GRADIENT_SPECIAL_STOP_START].c.b=0;

	g->data[GRADIENT_SPECIAL_STOP_END].x = 1;
	g->data[GRADIENT_SPECIAL_STOP_END].c.r=255;
	g->data[GRADIENT_SPECIAL_STOP_END].c.g=255;
	g->data[GRADIENT_SPECIAL_STOP_END].c.b=255;

	g->data[2].x = 0.5;
	g->data[2].c.r=0;
	g->data[2].c.g=255;
	g->data[2].c.b=0;

	g->_stack_top=2;
}

static void mix_cp_color(cp_color *pix, double p, cp_color a, cp_color b) {
		pix->r = (unsigned char)((double)b.r * p + (double)a.r * (1-p));
		pix->g = (unsigned char)((double)b.g * p + (double)a.g * (1-p));
		pix->b = (unsigned char)((double)b.b * p + (double)a.b * (1-p));
}


void gradient_add_stop(gradient* g, double p) {
	g->data[g->_stack_top].x = p;
	g->data[g->_stack_top].c = gradient_compute_color_at(g,p);
	g->_stack_top++;
}

#define NO_STOP_FOUND -1

void gradient_del_stop(gradient*g, _gp* stop) {
		unsigned int i;
		int found = NO_STOP_FOUND;

		for(i=0; i<g->_stack_top; ++i) {
						_gp* check =  &g->data[i];
						if(check == stop) {
								found = i;
								break;
						}
		}

		if(found != NO_STOP_FOUND ) {
			unsigned int j;
			g->_stack_top--;
			for(j = 0; j< g->_stack_top;++j) {
				int src_idx =j;

				if(src_idx>=found)
						src_idx++;

				g->data[j] = g->data[src_idx];
			}
		}
}

cp_color gradient_compute_color_at(gradient* g, double p) {
	unsigned int i;

	_gp* min = 0;
	_gp* max = 0;
	cp_color ret = {0};

	if(p>1) p = 1;
	if(p<0) p = 0;

	for(i=0; i<g->_stack_top; ++i) {
			_gp* cur = &g->data[i];
			if( cur->x == p) {
					return cur->c;
			}
			if( cur->x < p )
			{
					if( min == 0){
							min = cur;
							continue;
					}
					if( cur->x > min->x) {
							min = cur;
							continue;
					}
			}	
	}

	for(i=0; i<g->_stack_top; ++i) {
		_gp* cur = &g->data[i];
		if( cur->x > p )
		{
				if( max == 0){
						max = cur;
						continue;
				}
				if( cur->x < max->x) {
						max = cur;
						continue;
				}
		}	
	}

	p -= min->x;
	p /= (max->x - min->x);

	mix_cp_color(&ret,p,min->c,max->c);
	return ret;
}

