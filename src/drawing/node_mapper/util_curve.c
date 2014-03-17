#include <math.h>
#include "util_curve.h"

void init_curve(curve* c) {
	c->data[CURVE_SPECIAL_POINT_START].x = 0;
	c->data[CURVE_SPECIAL_POINT_START].y = 0;

	c->data[CURVE_SPECIAL_POINT_END].x = 1;
	c->data[CURVE_SPECIAL_POINT_END].y = 1;

	c->_stack_top=2;
}

double interp(double x, double a, double b) {
		x*=3.14159265359;
		x = 1-((cos(x)+1)/2);
		return (x*b)+( (1-x) * a);
}

void curve_add_point(curve* c, double p,double y) {
	c->data[c->_stack_top].x = p;
	c->data[c->_stack_top].y = y;
	c->_stack_top++;
}

#define NO_POINT_FOUND -1

void curve_del_point(curve* c, _cp* stop) {
		int i;
		int found = NO_POINT_FOUND;


		for(i=0; i<c->_stack_top; ++i) {
						_cp* check =  &c->data[i];
						if(check == stop) {
								found = i;
								break;
						}
		}
		if(found != NO_POINT_FOUND ) {
			int j;
			c->_stack_top--;
			for(j = 0; j< c->_stack_top;++j) {
				int src_idx =j;

				if(src_idx>=found)
						src_idx++;

				printf("setting data[%d] to source[%d]\n",j,src_idx);
				c->data[j] = c->data[src_idx];
			}
		}
}

double curve_compute_y_at(curve* c, double p) {

	int i;

	_cp* min = 0;
	_cp* max = 0;

	if(p>1) p = 1;
	if(p<0) p = 0;

	for(i=0; i<c->_stack_top; ++i) {
			_cp* cur = &c->data[i];
			if( cur->x == p) {
					return cur->y;
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

	for(i=0; i<c->_stack_top; ++i) {
		_cp* cur = &c->data[i];
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

	return interp(p,min->y,max->y);

}

