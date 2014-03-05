#ifndef _LOGFILE__
#define __LOGFILE__

#include <stdio.h>

void initLog();
void dropLog();

void log_msg(char* message);

FILE* getLogfile();

#endif
