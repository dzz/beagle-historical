#ifndef __GFX_LABEL__
#define __GFX_LABEL__

#include "texture.h"

typedef struct {
    gfx_texture *texture;
    int w;
    int h; 
    int _cursor; 
    int _set;
} gfx_label;

void label_generate(gfx_label* label);
void label_set_text(gfx_label* label, char* text);
void label_drop(gfx_label* label);
void initLabels();
void dropLabels();

#endif
