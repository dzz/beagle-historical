#ifndef __CTT2__
#define __CTT2__

#include <stdio.h>
#include <SDL.h>

__declspec( dllexport) void __stdcall makewin();
__declspec( dllexport) void __stdcall origin_init();
void startLog();
FILE *getLogfile();
void closeLog();
SDL_Surface *getDrawingContext();
void updateDrawingContext();

#define KEYFRAME_MODE_RECORD 1
#define KEYFRAME_MODE_INSERT 0

unsigned int getKeyframingMode();
void toggleKeyframingMode();

#endif

