#include <stdio.h>

#ifndef OGL_LOG

    #define OGL_RECV    "acquired"
    #define OGL_DROP    "dropped"

    #define PRINTOGL_MSGS

    #ifdef PRINTOGL_MSGS
        #define OGL_OBJ(object,id,mode) //printf("GL:%-12s - ID:%-3x - MODE:%-10s\n",object,id,mode)
        #define OGL_SHADOP(vshad,fshad) //printf("GL: SHADEROPS ON: %s, %s\n",vshad,fshad)
    #else
        #define OGL_OBJ(object,id,mode) 
        #define OGL_SHADOP(vshad,fshad)
    #endif

#endif
