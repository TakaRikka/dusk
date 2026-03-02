#include <revolution/dsp.h>

#include "__dsp.h"

void __DSP_debug_printf(const char* fmt, ...) {}

DSPTaskInfo* __DSPGetCurrentTask(void) {
    return __DSP_curr_task;
}
