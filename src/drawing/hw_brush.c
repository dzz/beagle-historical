#include <GL/glew.h>

#include "../system/ctt2.h"
#include "hw_brush.h"

void initHwBrush(){
    GLchar *vertexsource, *fragmentsource;
    GLuint vertexshader, fragmentshader;
    GLuint shaderprogram;

    glewInit();

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
}
