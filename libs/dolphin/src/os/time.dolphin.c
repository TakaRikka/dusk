#include <dolphin/dolphin.h>
#include <dolphin/os.h>

#include "__os.h"

OSTime __get_clock(void) {
    return __OSGetSystemTime();
}

u32 __get_time(void) {
    return OSTicksToSeconds(OSGetTime()) - 0x43E83E00;
}

int __to_gm_time(void) {
    return 0;
}
