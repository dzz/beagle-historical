#ifndef __UTIL_GRADIENT__
#define __UTIL_GRADIENT__
#include "../../colors/colors.h"

#define MAX_GRADIENT_POINTS 64

typedef struct _gp _gp;

struct _gp{
		double x;
		cp_color c;
		_gp* next;
};

typedef struct {
	unsigned int _stack_top;
	_gp data[MAX_GRADIENT_POINTS];
} gradient;

cp_color gradient_compute_color_at(gradient* g, double p);

#endif
