#ifndef __STYLUS__

typedef struct {
		int x;
		int y;
		double pressure;
} stylusState;

typedef stylusState stylusPacket;


void updateStylus(stylusPacket packet);
stylusState getStylusState( void );
void resetStylusState(void);
double stylusFilter_getFilteredPressure();

#define __STYLUS__
#endif
