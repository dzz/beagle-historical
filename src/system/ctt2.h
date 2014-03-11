#ifndef __CTT2__
#define __CTT2__

#include <SDL.h>

void updateDrawingContext();
void updateViewingSurface();
SDL_Surface *getViewingSurface();

#define KEYFRAME_MODE_RECORD 1
#define KEYFRAME_MODE_INSERT 0

unsigned int getKeyframingMode(void);
void toggleKeyframingMode(void);
void ctt2_insertkeyframe(void);

int client_get_screen_mousex();
int client_get_screen_mousey();

typedef unsigned int SYSTEM_SIGNAL;

#define SYSSIG_QUIT_CTT2 1

#endif
