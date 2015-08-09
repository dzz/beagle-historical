#ifndef __CTT2__
#define __CTT2__

#include <SDL.h>

unsigned int host_get_screen_width();
unsigned int host_get_screen_height();

typedef unsigned int CTT2_RT_SIGNAL;
typedef unsigned int CTT2_RT_FLAG;

#define CTT2_RT_TERMINATED (1)

void DIRTY_DISPLAY_ABORT();

#define HARDFAIL_CONSOLE DIRTY_DISPLAY_ABORT(); printf("\n***\npress any key\n"); getch();exit(1);


#include "rt_module_codes.h"


#endif
