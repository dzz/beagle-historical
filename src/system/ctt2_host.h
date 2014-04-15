#ifndef __CTT2_HOST__
#define __CTT2_HOST__

#define API_FAILURE 1
#define API_NOFAILURE 0

int api_init();
int api_tick();
void api_drop();

#endif
