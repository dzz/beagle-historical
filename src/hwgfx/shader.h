#ifndef __GFX_SHADER__
#define __GFX_SHADER__

#include <GLXW/glxw.h>

typedef struct {
    GLuint shader_id;
    GLuint vert_shader_id;
    GLuint frag_shader_id;
    char * vert_name;
    char * frag_name;
} gfx_shader;


void shader_load(gfx_shader* shader, const char* v_src_path, const char* f_src_path );
void shader_bind(gfx_shader* shader);
void shader_drop(gfx_shader* shader);
void shader_bind_vec4 (gfx_shader* shader, const char* param, float x, float y, float z, float w);
void shader_bind_vec3 (gfx_shader* shader, const char* param, float x, float y, float z);
void shader_bind_vec2 (gfx_shader* shader, const char* param, float x, float y);
void shader_bind_float (gfx_shader* shader, const char* param, float x);

gfx_shader* shader_get_bound();

#endif

