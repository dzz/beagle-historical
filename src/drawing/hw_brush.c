#include <GL/glew.h>

#include "../system/ctt2.h"
#include "hw_brush.h"

typedef struct {
    GLuint texture;

} brush_context;

brush_context _context;

void createBrushContext(brush_context ctxt) {
    glGenTextures(1,&ctxt.texture);
}

void destroyBrushContext(brush_context ctxt) {
    glDeleteTextures(1,&ctxt.texture);
}

void initHwBrush(){
    GLchar *vertexsource, *fragmentsource;
    GLuint vertexshader, fragmentshader;
    GLuint shaderprogram;
    glewInit();

    createBrushContext(_context);

    vertexsource = read_file("shaders/simple.vert.glsl");
    fragmentsource = read_file("shaders/simple.frag.glsl");

    vertexshader = glCreateShader(GL_VERTEX_SHADER);
    fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
    glCompileShader(vertexshader);


    glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
    glCompileShader(fragmentshader);

    shaderprogram = glCreateProgram();

    glAttachShader(shaderprogram, vertexshader);
    glAttachShader(shaderprogram, fragmentshader);

    glLinkProgram(shaderprogram);
    glUseProgram(shaderprogram);

    glUseProgram(0);
    glDetachShader(shaderprogram, vertexshader);
    glDetachShader(shaderprogram, fragmentshader);
    glDeleteProgram(shaderprogram);
    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);

    free(vertexsource);
    free(fragmentsource);
}

void dropHwBrush(){ 
    destroyBrushContext(_context);
}
