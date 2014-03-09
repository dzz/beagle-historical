#include "canvas.h"
#include "toolbar.h"
#include "colorPicker.h"
#include "../drawing/drawingSurfaces.h"
#include "../drawing/drawingContext.h"

unsigned int canvas_dispatch_mousedown(int x,int y) {
		if( get_selected_tool() == TOOL_EYEDROPPER ) {

			setCurColor( sample_surface( getDrawingContext(), x, y ) );
				toolbar_revert_to_previous_tool();
				return CANVAS_DISPATCH_TERMINATED;
		}

		return CANVAS_DISPATCH_CONTINUE;
}

