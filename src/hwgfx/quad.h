#include "texture.h"

typedef struct {
    float w;
    float h;
    float center_x;
    float center_y;
    float rot;
    float scale;
} gfx_quad;

void initQuads();
void quad_render( gfx_quad* quad, gfx_texture* texture);
void dropQuads();


