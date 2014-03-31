#include <math.h>
#include <stdio.h>

#include "../system/ctt2.h"

#include "../hardware/hw_run_vars.h"

#include "../drawing/brush.h"
#include "../drawing/node_mapper.h"

#include "../document/animation.h"

#include "stylus.h"

static stylusState current_stylus_frame = {0};
static stylusState previous_stylus_frame = {0};

void run_stroke_logic( void );

static double filteredPressure = 0;

void stylusFilter_apply_pressure_impulse(double p) {
			const double a = 0.9;
			const double b = 0.1;
			//alternate coefficients if we are in emulated stylus mode
			const double a_emu = 0.93;
			const double b_emu = 0.07;

			filteredPressure = p;
			return;
			if( HW_RUN_VAR_TABLET_CONNECTED == TABLET_CONNECTED ) {
					filteredPressure = filteredPressure *a +p*b;
			} else {
					filteredPressure = filteredPressure *a_emu +p*b_emu;
			}
}

double stylusFilter_getFilteredPressure() {
	return filteredPressure;
}

void updateStylus(stylusPacket packet) {
    double orientation;

	previous_stylus_frame = current_stylus_frame;
	current_stylus_frame.x = packet.x;
	current_stylus_frame.y = packet.y;
	current_stylus_frame.pressure = packet.pressure;
	current_stylus_frame.timestamp = packet.timestamp;
	stylusFilter_apply_pressure_impulse( packet.pressure );


    {
        double d_x = current_stylus_frame.x - previous_stylus_frame.x;
        double d_y = current_stylus_frame.y - previous_stylus_frame.y;
        orientation = atan2(d_x,d_y); 
    }

	node_mapper_apply_input( 
            stylusFilter_getFilteredPressure(), 
            packet.timestamp, 
            packet.azimuth,
            orientation
            );

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
				if(previous_stylus_frame.pressure>0){
					filteredPressure = 0;
					brush_render_stylus_stroke(previous_stylus_frame, current_stylus_frame);
					brush_end_stroke();
				}
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
