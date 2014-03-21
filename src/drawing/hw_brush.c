#include <GL/glew.h>

#include "../system/ctt2.h"
#include "hw_brush.h"

#define CONTEXT_SIZE 2048

typedef struct {
    GLuint texture_id;
    GLchar *vertex_src;
    GLchar *frag_src;
    GLuint shader_id;
    GLuint vert_shader_id;
    GLuint frag_shader_id;
} brush_context;

brush_context _context;

void createBrushContext(brush_context ctxt) {
    glGenTextures(1,&ctxt.texture_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,ctxt.texture_id);

    {
        unsigned char* image = malloc( CONTEXT_SIZE*CONTEXT_SIZE*4,sizeof(unsigned char));
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,CONTEXT_SIZE,CONTEXT_SIZE,0,
                GL_RGBA, GL_UNSIGNED_BYTE,image);
        free(image);
    }

    ctxt.vertex_src = read_file("shaders/simple.vert.glsl");
    ctxt.frag_src = read_file("shaders/simple.frag.glsl");
    ctxt.vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
    ctxt.frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(ctxt.vert_shader_id, 1, (const GLchar**)&ctxt.vertex_src, 0);
    glCompileShader(ctxt.vert_shader_id);
    glShaderSource(ctxt.frag_shader_id, 1, (const GLchar**)&ctxt.frag_src, 0);
    glCompileShader(ctxt.frag_shader_id);
    ctxt.shader_id = glCreateProgram();
    glAttachShader(ctxt.shader_id, ctxt.vert_shader_id);
    glAttachShader(ctxt.shader_id, ctxt.frag_shader_id);
    glLinkProgram(ctxt.shader_id);
}

void destroyBrushContext(brush_context ctxt) {
    glDeleteTextures(1,&ctxt.texture_id);
    free(ctxt.vertex_src);
    free(ctxt.frag_src);
    glDeleteProgram(ctxt.shader_id);
    glDeleteShader(ctxt.vert_shader_id);
    glDeleteShader(ctxt.frag_shader_id);
}

void invoke_brush_context(brush_context ctxt) {
    glUseProgram(ctxt.shader_id);
}

void detach_brush_context(brush_context ctxt) {
    glDetachShader(ctxt.shader_id, ctxt.vert_shader_id);
    glDetachShader(ctxt.shader_id, ctxt.frag_shader_id);
}

void initHwBrush(){
    glewInit();
    createBrushContext(_context);
    glUseProgram(0);
}

void dropHwBrush(){ 
    destroyBrushContext(_context);
}
