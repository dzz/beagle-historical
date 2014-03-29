#include <stdio.h>
#include "files.h"

char* read_file(char *file)
{
    FILE *fptr;
    long length;
    char *buf;

    fptr = fopen(file, "rb"); 
    if (!fptr) 
        return NULL;
    fseek(fptr, 0, SEEK_END); 
    length = ftell(fptr); 
    buf = (char*)malloc(length+1); 
    fseek(fptr, 0, SEEK_SET); 
    fread(buf, length, 1, fptr); 
    fclose(fptr); 
    buf[length] = 0; 
    return buf; 
}
