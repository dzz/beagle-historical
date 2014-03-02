#include <stdio.h>

#include "stylus.h"
#include "../system/ctt2.h"
#include "../drawing/brush.h"
#include "../document/animation.h"

stylusState gStylusState = {0};
stylusState gSSLastFrame = {0};

void runStylusLogic( void );

static double filteredPressure = 0;

void stylusFilter_apply_pressure_impulse(double p) {
			const double a = 0.8;
			const double b = 0.2;
			filteredPressure = filteredPressure *a +p*b;
}

double stylusFilter_getFilteredPressure() {
	return filteredPressure;
}
void updateStylus(stylusPacket packet) {

	gSSLastFrame = gStylusState;
	gStylusState.x = packet.x;
	gStylusState.y = packet.y;
	gStylusState.pressure = packet.pressure;
	stylusFilter_apply_pressure_impulse( packet.pressure );

	runStylusLogic();
}

void runStylusLogic( void ) {
		if( (gSSLastFrame.pressure>0) && (gStylusState.pressure > 0)) {
			brushPaint(gSSLastFrame, gStylusState);

		} else {
				brushReset();
		}
}

void resetStylusState(void) {
		gStylusState.pressure = 0;
		gSSLastFrame.pressure = 0;
		brushReset();
}

stylusState getStylusState() {
	return gStylusState;
}
