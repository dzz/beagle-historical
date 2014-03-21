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

    GLuint vert_array;
    GLuint vert_buffer;

} brush_context;

brush_context _context;

const GLfloat verts[4][2] = {
    {  0.0,  1.0  }, /* Top point */
    {  1.0,  0.0  }, /* Right point */
    {  0.0, -1.0  }, /* Bottom point */
    { -1.0,  0.0  } }; /* Left point */

void createBrushContext(brush_context ctxt) {
    /* bigass texture where we will draw to. when the brush is lifted
     * it will composite down to the drawingContext and get cleared */

    glGenTextures(1,&ctxt.texture_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,ctxt.texture_id);

    {
        unsigned char* image = malloc( CONTEXT_SIZE*CONTEXT_SIZE*4,sizeof(unsigned char));
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,CONTEXT_SIZE,CONTEXT_SIZE,0,
                GL_RGBA, GL_UNSIGNED_BYTE,image);
        free(image);
    }

    /* shaders */
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
    /* gemoetry */

    glGenVertexArrays(1, &ctxt.vert_array);
    glBindVertexArray(ctxt.vert_array);
    glGenBuffers(1, &ctxt.vert_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, ctxt.vert_buffer);

#define COORDINATE_ATTRIBUTE_INDEX  0
#define COLOR_ATTRIBUTE_INDEX 1

#define FLOATS_PER_VERT 2

#define NOT_NORMALIZED 0
#define NO_STRIDE 0
#define NO_POINTER_OFFSET 0

    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), verts, GL_STATIC_DRAW);

    glVertexAttribPointer(COORDINATE_ATTRIBUTE_INDEX, 
            FLOATS_PER_VERT, GL_FLOAT, NOT_NORMALIZED, 
            NO_STRIDE, 
            NO_POINTER_OFFSET); 

    //would bind uv etc. here
    glEnableVertexAttribArray(COORDINATE_ATTRIBUTE_INDEX);

}

void destroyBrushContext(brush_context ctxt) {
    glDeleteTextures(1,&ctxt.texture_id);
    free(ctxt.vertex_src); 
    free(ctxt.frag_src);
    glDetachShader(ctxt.shader_id, ctxt.vert_shader_id);
    glDetachShader(ctxt.shader_id, ctxt.frag_shader_id);
    glDeleteProgram(ctxt.shader_id);
    glDeleteShader(ctxt.vert_shader_id);
    glDeleteShader(ctxt.frag_shader_id);

    glDeleteVertexArrays(1,&ctxt.vert_array);
    glDeleteBuffers(1,&ctxt.vert_buffer);
}

void invoke_brush_context(brush_context ctxt) {
    glUseProgram(ctxt.shader_id);
}

void detach_brush_context(brush_context ctxt) {
}

void initHwBrush(){
    glewInit();
    createBrushContext(_context);
    glUseProgram(0);
}

void dropHwBrush(){ 
    destroyBrushContext(_context);
}

void renderBrushContext(brush_context ctxt) {
    glBindVertexArray(ctxt.vert_array);

}
