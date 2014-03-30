#include <GL/glew.h>
#include "blend_control.h"

unsigned static int _blending = 0;

void _blend_set_mode(unsigned int mode) {
    switch(mode) {
        case BLENDMODE_OVER:
               glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
    }
}
void blend_enter(unsigned int mode) {
    if(_blending == 1) {
        printf("maximum blend depth exceeded");
        exit(1);
    }
    _blend_set_mode(mode);
    glEnable(GL_BLEND);
}


void blend_exit() {
    if(_blending !=1) {
        printf("trying to exit blending but no blending set");
    }
    glDisable(GL_BLEND);
    _blending = 0;
}

