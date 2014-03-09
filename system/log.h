#ifndef _LOGFILE__
#define __LOGFILE__

#define LOG_MESSAGES_TO_STDOUT 1

#include <stdio.h>

void initLog();
void dropLog();

void log_msg(char* message);

FILE* getLogfile();

#endif
