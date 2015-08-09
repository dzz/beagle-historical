#include "log.h"
#include <stdarg.h>
#include <malloc.h>
#include "rt_module_codes.h"

#define LOG_FORMATTING_BUFFER_SIZE 1024
static FILE* logfile;
static char* formatting_buffer;
static unsigned int initialized = 0;

unsigned int initLog() {
	logfile = fopen("log.txt","w+");
	if(!logfile)
		return MODULE_FAILURE;
    formatting_buffer = (char*)malloc(sizeof(char)*LOG_FORMATTING_BUFFER_SIZE);
	if(!formatting_buffer)
		return MODULE_FAILURE;

	initialized = 1;
	return MODULE_LOADED;
}

FILE *getLogfile(){
	return logfile;
}

void dropLog() {
	initialized = 0;
    free(formatting_buffer);
	fclose(logfile);
}

void log_message(unsigned int system, unsigned int level, const char* message, ...) {

	if(!initialized) {
		va_list args;
        va_start(args, message);
		printf("(detached):");
		vprintf(message, args);
		printf("\n");
        va_end(args);
		return;
	}
    if( (level & LOG_LEVEL) == 0 ) {
        return;
    }
    else {
        const char* format = "%s(%x):%s\n";
        char* system_prefix = ctt2_module_from_code(system);
        va_list args;
        va_start(args, message);
        vsnprintf(formatting_buffer, LOG_FORMATTING_BUFFER_SIZE, message, args);
        va_end(args);

		printf(format, system_prefix, system, formatting_buffer);

        if(LOG_TARGET & LOG_TARGET_FILE) {
            fprintf(logfile, format, system_prefix, system, formatting_buffer);
        }
        if(LOG_TARGET & LOG_TARGET_STDOUT) {
			printf(format, system_prefix, system, formatting_buffer);
        }
    }
}
