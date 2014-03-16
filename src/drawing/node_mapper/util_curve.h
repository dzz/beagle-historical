#ifndef __UTIL_CURVE_H__
#define __UTIL_CURVE_H__

#define MAX_CURVE_POINTS 64

typedef struct {
		double x;
		double y;
} curve_point;

typedef curve_point[MAX_CURVE_POINTS] mapping_curve;

#endif
