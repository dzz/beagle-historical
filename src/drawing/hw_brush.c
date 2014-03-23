#include <stdlib.h>
#include <math.h>
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

void createBrushContext(brush_context *ctxt) {
    /* bigass texture where we will draw to. when the brush is lifted
     * it will composite down to the drawingContext and get cleared */

    glGenTextures(1,&ctxt->texture_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,ctxt->texture_id);

    {
        unsigned char* image = malloc( CONTEXT_SIZE*CONTEXT_SIZE*4,
                sizeof(unsigned char));

        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,CONTEXT_SIZE,CONTEXT_SIZE,0,
                GL_RGBA, GL_UNSIGNED_BYTE,image);
        free(image);
    }

    /* shaders */



    {
        int iv;

        ctxt->vertex_src = read_file("shaders/simple.vert.glsl");
        ctxt->frag_src = read_file("shaders/simple.frag.glsl");

        ctxt->vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
        ctxt->frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(ctxt->vert_shader_id, 1, (const GLchar**)&ctxt->vertex_src, 0);
        glCompileShader(ctxt->vert_shader_id);
        glGetShaderiv(ctxt->vert_shader_id, GL_COMPILE_STATUS, &iv);
        if(iv == 0 ){
            int maxLength;
            char* fragmentInfoLog;
            printf("error compiling vert shader\n");
            glGetShaderiv(ctxt->vert_shader_id, GL_INFO_LOG_LENGTH, &maxLength);
            fragmentInfoLog = (char *)malloc(maxLength);
            glGetShaderInfoLog(ctxt->vert_shader_id, maxLength, &maxLength, fragmentInfoLog);
            printf(fragmentInfoLog);
            free(fragmentInfoLog);
        }

        glShaderSource(ctxt->frag_shader_id, 1, (const GLchar**)&ctxt->frag_src, 0);
        glCompileShader(ctxt->frag_shader_id);
        glGetShaderiv(ctxt->frag_shader_id, GL_COMPILE_STATUS, &iv);
        if(iv == 0 ){
            printf("error compiling frag shader\n");
        }

        ctxt->shader_id = glCreateProgram();
        glAttachShader(ctxt->shader_id, ctxt->vert_shader_id);
        glAttachShader(ctxt->shader_id, ctxt->frag_shader_id);

        glBindAttribLocation(ctxt->shader_id, 0, "in_Position");
        glLinkProgram(ctxt->shader_id);

        glGetProgramiv(ctxt->shader_id, GL_LINK_STATUS, (int *)&iv);
        if(iv == 0) {
            printf("error linking shader\n");
            
        }
    }

    /* gemoetry */
    glGenVertexArrays(1, &ctxt->vert_array);
    glBindVertexArray(ctxt->vert_array);
    glGenBuffers(1, &ctxt->vert_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, ctxt->vert_buffer);
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

void destroyBrushContext(brush_context *ctxt) {
    glDeleteTextures(1,&ctxt->texture_id);
    free(ctxt->vertex_src); 
    free(ctxt->frag_src);
    glDetachShader(ctxt->shader_id, ctxt->vert_shader_id);
    glDetachShader(ctxt->shader_id, ctxt->frag_shader_id);
    glDeleteProgram(ctxt->shader_id);
    glDeleteShader(ctxt->vert_shader_id);
    glDeleteShader(ctxt->frag_shader_id);
    glDeleteVertexArrays(1,&ctxt->vert_array);
    glDeleteBuffers(1,&ctxt->vert_buffer);
}

void initHwBrush(){
    createBrushContext(&_context);
}

void dropHwBrush(){ 
    glUseProgram(0);
    destroyBrushContext(&_context);
}

int oscillator = 0;

void renderBrushContext(brush_context ctxt) {
    int i;
    float fr = (float)(oscillator%2)/10.0f;

    oscillator ++;

    glUseProgram(ctxt.shader_id);

    glClearColor(1-fr,fr,fr,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(ctxt.vert_array);
    for(i=2;i<4;++i) {
        glDrawArrays(GL_LINE_LOOP, 0, i);
    } 
}
void debug_drawscene(void);

void renderHwBrush() {
       renderBrushContext(_context);
} 

