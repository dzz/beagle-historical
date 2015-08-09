#include "log.h"
#include <stdarg.h>
#include <malloc.h>

#define LOG_FORMATTING_BUFFER_SIZE 1024
static FILE* logfile;
static char* formatting_buffer;

void initLog() {
	logfile = fopen("log.txt","w+");
    formatting_buffer = malloc(sizeof(char)*LOG_FORMATTING_BUFFER_SIZE);
}

FILE *getLogfile(){
	return logfile;
}

void dropLog() {
    free(formatting_buffer);
	fclose(logfile);
}

void log_message(unsigned int system, unsigned int level, const char* message, ...) {

    if( (level & LOG_LEVEL) == 0 )
        return;
    else {
        const char* format = "(%x):%s";
        va_list args;
        va_start(args, message);
        vsnprintf(formatting_buffer, LOG_FORMATTING_BUFFER_SIZE, message, args);
        va_end(args);

        if(LOG_TARGET & LOG_TARGET_FILE)
            fprintf(logfile, format, formatting_buffer);
        if(LOG_TARGET & LOG_TARGET_STDOUT)
			printf(format, formatting_buffer);
    }
}
