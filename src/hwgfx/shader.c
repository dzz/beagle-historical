#include <GL/glew.h>

#include "../system/ctt2.h"
#include "../system/log.h"
#include "shader.h"

void shader_load(gfx_shader* shader, const char* v_src_path, 
        const char* f_src_path ){

    char* vertex_src;
    char* frag_src;
    int iv;

    vertex_src = read_file(v_src_path);
    frag_src = read_file(f_src_path);

    shader->vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
    shader->frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader->vert_shader_id, 1, (const GLchar**)&vertex_src, 0);
    glCompileShader(shader->vert_shader_id);
    glGetShaderiv(shader->vert_shader_id, GL_COMPILE_STATUS, &iv);
    if(iv == 0 ){
        int maxLength;
        char* fragmentInfoLog;
        printf("error compiling vert shader\n");
        glGetShaderiv(shader->vert_shader_id, GL_INFO_LOG_LENGTH, &maxLength);
        fragmentInfoLog = (char *)malloc(maxLength);
        glGetShaderInfoLog(shader->vert_shader_id, maxLength, &maxLength, fragmentInfoLog);
        log_msg(fragmentInfoLog);
        free(fragmentInfoLog);
        exit(27);
    }

    glShaderSource(shader->frag_shader_id, 1, (const GLchar**)&frag_src, 0);
    glCompileShader(shader->frag_shader_id);
    glGetShaderiv(shader->frag_shader_id, GL_COMPILE_STATUS, &iv);
    if(iv == 0 ){
        int maxLength;
        char* fragmentInfoLog;
        printf("error compiling frag shader\n");
        glGetShaderiv(shader->frag_shader_id, GL_INFO_LOG_LENGTH, &maxLength);
        fragmentInfoLog = (char *)malloc(maxLength);
        glGetShaderInfoLog(shader->frag_shader_id, maxLength, &maxLength, fragmentInfoLog);
        log_msg(fragmentInfoLog);
        free(fragmentInfoLog);
        exit(9);
    }

    shader->shader_id = glCreateProgram();
    glAttachShader(shader->shader_id, shader->vert_shader_id);
    glAttachShader(shader->shader_id, shader->frag_shader_id);

    glBindAttribLocation(shader->shader_id, 0, "in_Position");
    glLinkProgram(shader->shader_id);

    glGetProgramiv(shader->shader_id, GL_LINK_STATUS, (int *)&iv);
    if(iv == 0) {
        printf("error linking shader\n");
        exit(2);
    }
    free(vertex_src);
    free(frag_src);
}

void shader_bind(gfx_shader* shader){
    glUseProgram(shader->shader_id);
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

void shader_drop(gfx_shader* shader) {
    glUseProgram(0); 
    glDetachShader(shader->shader_id, shader->vert_shader_id);
    glDetachShader(shader->shader_id, shader->frag_shader_id);
    glDeleteProgram(shader->shader_id);
    glDeleteShader(shader->vert_shader_id);
    glDeleteShader(shader->frag_shader_id);
}


