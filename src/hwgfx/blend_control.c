#include <GL/glew.h>
#include "blend_control.h"

#define BLENDMODE_STACK_DEPTH 8
//count the number of clients who enabled blend
static unsigned int blend_refcount = 0;
static unsigned int current_blendmode = BLENDMODE_OVER;
static unsigned int blendmode_stack [ BLENDMODE_STACK_DEPTH ];

void _blend_set_mode(unsigned int mode) {
    current_blendmode = mode;
    switch(mode) {
        case BLENDMODE_OVER:
               glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
    }
}
void blend_enter(unsigned int mode) {
    _blend_set_mode(mode);
    if(blend_refcount == 0)
        glEnable(GL_BLEND);
    blendmode_stack[blend_refcount] = current_blendmode;
    blend_refcount++;
    if(blend_refcount == BLENDMODE_STACK_DEPTH) {
        printf("maximum blend depth exceeded");
        exit(1);
    }
}


void blend_exit() {
    if(blend_refcount == 1) {
        glDisable(GL_BLEND);
        blend_refcount = 0;
    } else {
        blend_refcount--;
    }
    _blend_set_mode( blendmode_stack[blend_refcount] );
}
