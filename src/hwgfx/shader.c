#ifdef _WIN32
#include <conio.h>
#endif

#include <stdio.h>
#include <GLXW/glxw.h>
#include "OGL_OBJ.h"

#include "../system/ctt2.h"
#include "../system/files.h"
#include "../system/log.h"
#include "shader.h"


void _shader_err(GLuint shader_id, char* source) {
    int maxLength;
    char* infoLog;
    log_message(CTT2_INT_HWGFX, LOG_LEVEL_ERROR, "error compiling shader: %s\n", source);
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &maxLength);
    infoLog = (char *)malloc(maxLength);
    glGetShaderInfoLog(shader_id, maxLength, &maxLength, infoLog);
    log_message(CTT2_INT_HWGFX, LOG_LEVEL_ERROR, (infoLog));
    free(infoLog);
}

void shader_compile(gfx_shader* shader, const char* vertex_src, const char* frag_src, const char* vert_name, const char* frag_name) {
    int iv;


    shader->vert_name = malloc( sizeof(char)*(strlen(vert_name)+1));
    shader->frag_name = malloc( sizeof(char)*(strlen(frag_name)+1));

    strcpy(shader->vert_name, vert_name);
    strcpy(shader->frag_name, frag_name);

    shader->vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
    shader->frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader->vert_shader_id, 1, (const GLchar**)&vertex_src, 0);
    glCompileShader(shader->vert_shader_id);
    glGetShaderiv(shader->vert_shader_id, GL_COMPILE_STATUS, &iv);
    if(iv == 0 ){
        _shader_err(shader->vert_shader_id, shader->frag_name);
    }

    glShaderSource(shader->frag_shader_id, 1, (const GLchar**)&frag_src, 0);
    glCompileShader(shader->frag_shader_id);
    glGetShaderiv(shader->frag_shader_id, GL_COMPILE_STATUS, &iv);
    if(iv == 0 ){
        _shader_err(shader->frag_shader_id, shader->vert_name);
    }

    shader->shader_id = glCreateProgram();
    glAttachShader(shader->shader_id, shader->vert_shader_id);
    glAttachShader(shader->shader_id, shader->frag_shader_id);

    glBindAttribLocation(shader->shader_id, 0, "in_Position");
    glLinkProgram(shader->shader_id);

    glGetProgramiv(shader->shader_id, GL_LINK_STATUS, (int *)&iv);
    if(iv == 0) {
        log_message(CTT2_INT_HWGFX, LOG_LEVEL_ERROR, "error linking shader");
    }

   
    OGL_OBJ("vertshad", shader->vert_shader_id, OGL_RECV);
    OGL_OBJ("fragshad", shader->frag_shader_id, OGL_RECV);
    OGL_OBJ("compshad", shader->shader_id,      OGL_RECV);
}

void shader_load(gfx_shader* shader, const char* v_src_path, const char* f_src_path ){
    char* vertex_src;
    char* frag_src;

    vertex_src = read_file((char*)v_src_path);
    frag_src = read_file((char*)f_src_path);

    shader_compile(shader, vertex_src, frag_src, v_src_path, f_src_path );
    free(vertex_src);
    free(frag_src);
}

static gfx_shader* _bound = 0;

void shader_bind(gfx_shader* shader){
    /* lazy binding - don't take the driver hit
     * if we're using this shader already */
    if(_bound != shader ) {
        glUseProgram(shader->shader_id);
        _bound = shader;
    }
}

gfx_shader* shader_get_bound() {
    return _bound;
}

void shader_bind_vec4(gfx_shader* shader, const char* param, float x, float y, float z, float w) {
   int loc = glGetUniformLocation( shader->shader_id, param );
   glUniform4f( loc, x,y,z,w );
}

void shader_bind_vec3(gfx_shader* shader, const char* param, float x, float y, float z) {
   glUniform3f( glGetUniformLocation( shader->shader_id, param ), x,y,z );
}

void shader_bind_vec2(gfx_shader* shader, const char* param, float x, float y) {
   glUniform2f( glGetUniformLocation( shader->shader_id, param ), x,y);
}

void shader_bind_float(gfx_shader* shader, const char* param, float x) {
   glUniform1f( glGetUniformLocation( shader->shader_id, param ), x);
}

void shader_bind_texture(gfx_shader* shader , const char* param, gfx_texture* texture) {
    glUniform1i(glGetUniformLocation(shader->shader_id, param), texture->bound_unit);
}

void shader_drop(gfx_shader* shader) {

    /*if our current shader is bound, unbind it
     * before attempting to delete GL resources */
    if(_bound == shader) {
        glUseProgram(0);
        _bound = 0;
    }

    glDetachShader(shader->shader_id, shader->vert_shader_id);
    glDetachShader(shader->shader_id, shader->frag_shader_id);
    glDeleteProgram(shader->shader_id);
    glDeleteShader(shader->vert_shader_id);
    glDeleteShader(shader->frag_shader_id);

    free(shader->vert_name);
    free(shader->frag_name);

    OGL_SHADOP(shader->vert_name, shader->frag_name);
    OGL_OBJ("vertshad", shader  ->vert_shader_id, OGL_DROP);
    OGL_OBJ("fragshad", shader  ->frag_shader_id, OGL_DROP);
    OGL_OBJ("compshad", shader  ->shader_id,      OGL_DROP);
}


