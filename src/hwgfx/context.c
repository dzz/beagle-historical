#include <GL/GLew.h>

#include "context.h"

int _vp[4] = {0}

int _unknown = 1;

void _validate_vp(){
    if(_unknown){
        glGetIntegerv(GL_VIEWPORT,_vp);
        _unknown = 0;
    }
    return _vp;
}

void gfx_viewport_set_dims(viewport_dims dims) {
    int i;

    glViewport( dims.x, 
                dims.y, 
                dims.w, 
                dims.h );
    _unknown = 1; 
}

viewport_dims   gfx_viewport_get_dims() {
    viewport_dims dims;

    dims.x = _vp[0];
    dims.y = _vp[1];
    dims.w = _vp[2];
    dims.h = _vp[3];
}

root_size gfx_get_root_size(){
    root_gfx_size gfx_size;
    gfx_size.w = 1920;
    gfx_size.h = 1080;
}
