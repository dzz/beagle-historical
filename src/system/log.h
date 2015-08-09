#ifndef _LOGFILE__
#define __LOGFILE__

//log everywhere
#define LOG_TARGET (4)

#define LOG_TARGET_STDOUT (1)
#define LOG_TARGET_FILE (2)

//log everything
#define LOG_LEVEL (8)

#define LOG_LEVEL_ERROR (1)
#define LOG_LEVEL_INFO (2)
#define LOG_LEVEL_WARNING (4)
#define LOG_LEVEL_DEBUG (8)


#include <stdio.h>
#include <stdarg.h>

void initLog();
void dropLog();

void log_message( unsigned int system, unsigned int level, const char* mesage, ... ); 

FILE* getLogfile();

#endif
