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

	g->data[2].x = 0.5;
	g->data[2].c.r=0;
	g->data[2].c.g=255;
	g->data[2].c.b=0;
	g->data[2].next = 0;

	g->_stack_top=2;
}

static void mix_cp_color(cp_color *pix, double p, cp_color a, cp_color b) {
		pix->r = (unsigned char)((double)b.r * p + (double)a.r * (1-p));
		pix->g = (unsigned char)((double)b.g * p + (double)a.g * (1-p));
		pix->b = (unsigned char)((double)b.b * p + (double)a.b * (1-p));
}


void gradient_add_stop(gradient* g, double p) {
//	printf("grad:%d  x:%f\n",g,p);

	g->data[g->_stack_top].x = p;
	g->data[g->_stack_top].c = gradient_compute_color_at(g,p);
	g->_stack_top++;

//	printf("created gradstop:%d  x:%f\n",g,p);
//	printf("stack_top:%d  x:%f\n",g->_stack_top);
}

#define NO_STOP_FOUND -1

void gradient_del_stop(gradient*g, _gp* stop) {
		int i;
		int found = NO_STOP_FOUND;

		printf("grad:%d  _gp deleting:%d\n",g,stop);
		printf("stack_top:%d  x:%f\n",g->_stack_top);

		for(i=0; i<g->_stack_top; ++i) {
						_gp* check =  &g->data[i];
						if(check == stop) {
								found = i;
								break;
						}
		}

		printf("idx %d to delete\n",found);

		if(found != NO_STOP_FOUND ) {
			int j;
			g->_stack_top--;
			for(j = 0; j< g->_stack_top;++j) {
				int src_idx =j;

				if(src_idx>=found)
						src_idx++;

				printf("setting data[%d] to source[%d]\n",j,src_idx);
				g->data[j] = g->data[src_idx];
			}
		}
		printf("finished delete\n");
		printf("stack_top:%d  x:%f\n",g->_stack_top);
}

cp_color gradient_compute_color_at(gradient* g, double p) {

	int i;

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

