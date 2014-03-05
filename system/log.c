#include "log.h"

static FILE* logfile;

void initLog() {
	logfile = fopen("log.txt","w+");
}

FILE *getLogfile(){
	return logfile;
}

void dropLog() {
	fclose(logfile);
}

void log_msg(char* message) {
		fprintf(logfile, message);
}
