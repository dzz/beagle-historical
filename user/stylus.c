#include <stdio.h>

#include "stylus.h"
#include "../system/ctt2.h"
#include "../drawing/brush.h"
#include "../document/animation.h"

stylusState gStylusState = {0};
stylusState gSSLastFrame = {0};

void runStylusLogic( void );

void updateStylus(stylusPacket packet) {

	gSSLastFrame = gStylusState;

	gStylusState.x = packet.x;
	gStylusState.y = packet.y;
	gStylusState.pressure = packet.pressure;
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
