#ifndef __CTT2__
#define __CTT2__

#include <stdio.h>
#include <SDL.h>

void startLog();
FILE *getLogfile();
void closeLog();
SDL_Surface *getDrawingContext();
void updateDrawingContext();

#define KEYFRAME_MODE_RECORD 1
#define KEYFRAME_MODE_INSERT 0

unsigned int getKeyframingMode();
void toggleKeyframingMode();

int client_get_screen_mousex();
int client_get_screen_mousey();


#endif

