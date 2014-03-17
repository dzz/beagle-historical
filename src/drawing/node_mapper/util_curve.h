#ifndef __UTIL_CURVE_H__
#define __UTIL_CURVE_H__

#define MAX_CURVE_POINTS 64

typedef struct {
		double x;
		double y;
} _cp;

typedef struct {
	unsigned int _stack_top;
	_cp data[MAX_CURVE_POINTS];
} curve;

#define CURVE_SPECIAL_POINT_START 0
#define CURVE_SPECIAL_POINT_END 1

void init_curve(curve* c);
void curve_add_point(curve* c, double p);
void curve_del_point(_cp* stop); 
double curve_compute_y_at(curve* c, double p);

#endif
