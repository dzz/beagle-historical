#include <stdio.h>
#include <windows.h>
#include "../system/log.h"
#include "hf_timer.h"
#include "../system/rt_module_codes.h"

LARGE_INTEGER Frequency;
double pcf;

unsigned int initTimer()
{
    QueryPerformanceFrequency(&Frequency);
    pcf = (double)Frequency.QuadPart;
    log_message(CTT2_RT_MODULE_TIMER, LOG_LEVEL_INFO, "freq:\t%i", Frequency.QuadPart);
    log_message(CTT2_RT_MODULE_TIMER, LOG_LEVEL_INFO, "time:\t%i", timer_get_ms());
    return MODULE_LOADED;
}

double timer_get_ms()
{
    LARGE_INTEGER CurrentTime;
    QueryPerformanceCounter(&CurrentTime);

	CurrentTime.QuadPart *= 1000;

    return (double)CurrentTime.QuadPart / pcf;
}

void dropTimer() {} 
