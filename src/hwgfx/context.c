#include <GLXW/glxw.h>

#include "../system/ctt2.h"
#include "context.h"

int _vp[4] = {0};

int _unknown = 1;

void _validate_vp(){
    if(_unknown == 1){
        glGetIntegerv(GL_VIEWPORT,_vp);
        _unknown = 0;
    }
}

void gfx_viewport_set_dims(viewport_dims dims) {

    glViewport( dims.x, 
                dims.y, 
                dims.w, 
                dims.h );
    _unknown = 1; 
}

viewport_dims   gfx_viewport_get_dims() {
    viewport_dims dims;
    
    _validate_vp();
    dims.x = _vp[0];
    dims.y = _vp[1];
    dims.w = _vp[2];
    dims.h = _vp[3];
    return dims;
}


root_gfx_size
	gfx_get_root_gfx_size(){
    root_gfx_size gfx_size;
    gfx_size.w = SCREEN_WIDTH;
    gfx_size.h = SCREEN_HEIGHT;
    return gfx_size;
}
