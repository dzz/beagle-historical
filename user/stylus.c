#include <stdio.h>

#include "stylus.h"
#include "../system/ctt2.h"
#include "../drawing/brush.h"
#include "../document/animation.h"

static stylusState currentStylusFrame = {0};
static stylusState previousStylusFrame = {0};

void runStylusLogic( void );

static double filteredPressure = 0;

void stylusFilter_apply_pressure_impulse(double p) {
			const double a = 0.4;
			const double b = 0.6;
			filteredPressure = filteredPressure *a +p*b;
}

double stylusFilter_getFilteredPressure() {
	return filteredPressure;
}
void updateStylus(stylusPacket packet) {

	previousStylusFrame = currentStylusFrame;
	currentStylusFrame.x = packet.x;
	currentStylusFrame.y = packet.y;
	currentStylusFrame.pressure = packet.pressure;
	stylusFilter_apply_pressure_impulse( packet.pressure );

	runStylusLogic();
}

void runStylusLogic( void ) {
		if( (previousStylusFrame.pressure>0) && (currentStylusFrame.pressure > 0)) {
			brushPaint(previousStylusFrame, currentStylusFrame);
		} else {
				brushReset();
		}
}

void resetStylusState(void) {
		currentStylusFrame.pressure = 0;
		previousStylusFrame.pressure = 0;
		filteredPressure = 0;
		brushReset();
}

stylusState getStylusState() {
	return currentStylusFrame;
}
