#include <stdio.h>

#include "stylus.h"
#include "../hardware/hw_run_vars.h"
#include "../system/ctt2.h"
#include "../drawing/brush.h"
#include "../document/animation.h"

static stylusState current_stylus_frame = {0};
static stylusState previous_stylus_frame = {0};

void run_stroke_logic( void );

static double filteredPressure = 0;

void stylusFilter_apply_pressure_impulse(double p) {
			const double a = 0.4;
			const double b = 0.6;
			//alternate coefficients if we are in emulated stylus mode
			const double a_emu = 0.93;
			const double b_emu = 0.07;

			if( HW_RUN_VAR_TABLET_CONNECTED == TABLET_CONNECTED ) {
					filteredPressure = filteredPressure *a +p*b;
			} else {
					//we're using a mouse and want to filter the
					//heavy 0/1 toggle in pressure
					filteredPressure = filteredPressure *a_emu +p*b_emu;
			}
}

double stylusFilter_getFilteredPressure() {
	return filteredPressure;
}

void updateStylus(stylusPacket packet) {
	previous_stylus_frame = current_stylus_frame;
	current_stylus_frame.x = packet.x;
	current_stylus_frame.y = packet.y;
	current_stylus_frame.pressure = packet.pressure;
	current_stylus_frame.timestamp = packet.timestamp;
	stylusFilter_apply_pressure_impulse( packet.pressure );

	run_stroke_logic();
}

void run_stroke_logic( void ) {
		if (current_stylus_frame.pressure > 0) {

				if(previous_stylus_frame.pressure == 0) 
						brush_begin_stroke(current_stylus_frame);

				if(previous_stylus_frame.pressure>0) 
						brush_render_stylus_stroke(previous_stylus_frame, current_stylus_frame);
		} 

		if (current_stylus_frame.pressure ==0) {	 
				if(previous_stylus_frame.pressure>0)
					brush_end_stroke();

		}
}

void resetStylusState(void) {
		current_stylus_frame.pressure = 0;
		previous_stylus_frame.pressure = 0;
		filteredPressure = 0;
		brush_end_stroke();
}

stylusState getStylusState() {
	return current_stylus_frame;
}
