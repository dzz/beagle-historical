#include "util_gradient.h"

void init_gradient(gradient* g) {
	g->data[0].x = 0;
	g->data[0].c.r=0;
	g->data[0].c.g=0;
	g->data[0].c.b=0;
	g->data[0].next=&g->data[1];

	g->data[1].x = 1;
	g->data[1].c.r=255;
	g->data[1].c.g=255;
	g->data[1].c.b=255;
	g->data[1].next = 0;

	g->_stack_top=2;
}

static void mix_cp_color(cp_color *pix, double p, cp_color a, cp_color b) {
		pix->r = (unsigned char)((double)b.r * p + (double)a.r * (1-p));
		pix->g = (unsigned char)((double)b.g * p + (double)a.g * (1-p));
		pix->b = (unsigned char)((double)b.b * p + (double)a.b * (1-p));
}

cp_color gradient_compute_color_at(gradient* g, double p) {

	int i;

	_gp* min = 0;
	_gp* max = 0;
	cp_color ret;

	for(i=0; i<g->_stack_top; ++i) {
			_gp* cur = &g->data[i];
			if(p==cur->x) {
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

