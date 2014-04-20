#include <GLXW/glxw.h>
#include "blend_control.h"

unsigned static int _blending = 0;

void _blend_set_mode(unsigned int mode) {
    switch(mode) {
        case BLENDMODE_OVER:
               glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
               glBlendEquation(GL_FUNC_ADD);
            break;
        case BLENDMODE_BRUSH_COMPOSITE:
               glBlendFuncSeparate(GL_ONE,GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE);
               glBlendEquationSeparate(GL_FUNC_ADD,GL_FUNC_ADD);
            break;
        case BLENDMODE_BLACKHOLE:
               glBlendFuncSeparate(GL_ONE_MINUS_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
                       GL_ONE,GL_ONE);
               glBlendEquation(GL_FUNC_ADD);
            break;
        case BLENDMODE_DAB_RENDERING:
               glBlendFuncSeparate(GL_SRC_ALPHA, GL_ZERO,
                       GL_ONE,GL_ONE);
               glBlendEquationSeparate(GL_FUNC_ADD,GL_MAX);
            break;
    }
}
void blend_enter(unsigned int mode) {
    if(_blending == 1) {
        printf("maximum blend depth exceeded\n");
        exit(1);
    }
    _blending = 1;
    _blend_set_mode(mode);
    glEnable(GL_BLEND);
}


void blend_exit() {
    if(_blending !=1) {
        printf("trying to exit blending but no blending set\n");
        exit(1);
    }
    glDisable(GL_BLEND);
    _blending = 0;
}

