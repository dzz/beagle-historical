#ifndef __UTIL_GRADIENT__
#define __UTIL_GRADIENT__
#include "../../colors/colors.h"

#define MAX_GRADIENT_POINTS 64

#define GRADIENT_SPECIAL_STOP_START 0
#define GRADIENT_SPECIAL_STOP_END 1

typedef struct _gp _gp;

struct _gp{
		double x;
		cp_color c;
};

typedef struct {
	unsigned int _stack_top;
	_gp data[MAX_GRADIENT_POINTS];
} gradient;


//note: gradients require data[0] and data[1] to be
//      located at 0 and 1

void init_gradient(gradient* g);
void gradient_add_stop(gradient* g, double p);
void gradient_del_stop(_gp* stop); 
cp_color gradient_compute_color_at(gradient* g, double p);

#endif
