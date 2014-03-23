#ifndef __GFX_TEXTURE__
#define __GFX_TEXTURE__

#include <GL/glew.h>

typedef struct {
    GLuint texture_id;
} gfx_texture;


//void texture_load(gfx_texture* texture, const char* path);
void texture_generate(gfx_texture* texture, int w,int h);
void texture_drop(gfx_texture* texture);
void texture_bind(gfx_texture* texture, int texture_unit);

#define TEX_UNIT_0 0

#endif
