#ifndef _LOGFILE__
#define __LOGFILE__

#define LOG_TARGET_STDOUT (1)
#define LOG_TARGET_FILE (2)
//log everywhere
#define LOG_TARGET (LOG_TARGET_STDOUT | LOG_TARGET_FILE )

#define LOG_LEVEL_ERROR (1)
#define LOG_LEVEL_WARNING (2)
#define LOG_LEVEL_INFO (4)
#define LOG_LEVEL_DEBUG (8)
//log everything
#define LOG_LEVEL (LOG_LEVEL_ERROR | LOG_LEVEL_WARNING | LOG_LEVEL_INFO | LOG_LEVEL_ERROR )

#include <stdio.h>
#include <stdarg.h>

#include "rt_module_codes.h"

unsigned int initLog();
void dropLog();

void log_message( unsigned int system, unsigned int level, const char* mesage, ... ); 
void log_client_message( unsigned int level, const char* mesage ); 

FILE* getLogfile();

#endif
