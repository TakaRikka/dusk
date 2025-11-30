//
// Translation Unit: JASReport
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASReport.h"
#include "JSystem/JAudio2/JASMutex.h"
#include "stdio.h"

static OSMutex sMutex;

static char* sBuffer;

static int sLineMax;

static int sLineCount;

static int sTop;

void JASReport(const char * str, ...) {
    if(sBuffer) {
        va_list vl;
        va_start(vl, str);
        JASMutexLock mutexLock(&sMutex);
        vsnprintf(sBuffer + (sTop * 64), 64, str, vl);
        va_end(vl);

        sTop++;
        if (sTop >= sLineMax)
            sTop = 0;

        if (sLineCount < sLineMax)
            sLineCount++;
    }
}
