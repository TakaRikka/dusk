//
// Translation Unit: JASReport
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASReport.h"
#include "JSystem/JAudio2/JASCalc.h"
#include "JSystem/JAudio2/JASMutex.h"
#include "JSystem/JKernel/JKRHeap.h"
#include <cstdio>

static OSMutex sMutex;

static char* sBuffer;

static int sLineMax;

static int sLineCount;

static int sTop;

void JASReportInit(JKRHeap* heap, int lineMax) {
    JUT_ASSERT(33, sBuffer == NULL);
    JUT_ASSERT(35, heap != NULL);
    OSInitMutex(&sMutex);
    sLineMax = lineMax;
    sBuffer = new (heap, 0) char[sLineMax * 64];
    JUT_ASSERT(41, sBuffer);
}

int JASReportGetLineMax() {
    return sLineMax;
}

int JASReportCopyBuffer(char *param_1,int lines) {
    if (!sBuffer) {
        return 0;
    }
    if (lines < 0) {
        lines = sLineMax;
    }
    char* dest = param_1;
    JASMutexLock lock(&sMutex);
    int i;
    int r30 = sTop - 1;
    char* src;
    for (i = 0; i < sLineCount && i < lines; i++) {
        if (r30 < 0) {
            r30 = sLineMax - 1;
        }
        src = &sBuffer[r30 * 64];
        JASCalc::bcopy(src, dest, 64);
        r30--;
        dest += 64;
    }
    return i;
}

void JASReport(const char * str, ...) {
    va_list vl;
    if(!sBuffer) {
        return;
    }

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
