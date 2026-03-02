#include <dolphin/vi.h>

#include "__vi.h"

static int lastError;

static int wait4ClkHigh(void) {
    int n;

    for (n = 0; n < 1000; n++) {
        if (__VIGetSCL() != 0) {
            return 1;
        }
    }

    lastError = 2;
    return 0;
}

static int sendSlaveAddr(u8 slaveAddr) {
    int i;

    __VISetSDA(0);
    __VISetSCL(0);

    for (i = 0; i < 8; i++) {
        if (slaveAddr & 0x80) {
            __VISetSDA(1);
        } else {
            __VISetSDA(0);
        }

        __VISetSCL(1);
        if (wait4ClkHigh() == 0) {
            return 0;
        }

        __VISetSCL(0);
        slaveAddr <<= 1;
    }

    __VISetSDA(1);
    __VISetSCL(1);

    if (wait4ClkHigh() == 0) {
        return 0;
    }

    if (__VIGetSDA() != 0) {
        lastError = 1;
        return 0;
    }

    __VISetSCL(0);
    return 1;
}

int __VISendI2CData(u8 slaveAddr, u8* pData, int nBytes) {
    s32 i;
    u8 data;

    if (sendSlaveAddr(slaveAddr) == 0) {
        return 0;
    }

    while (nBytes != 0) {
        data = *pData++;
        for (i = 0; i < 8; i++) {
            if (data & 0x80) {
                __VISetSDA(1);
            } else {
                __VISetSDA(0);
            }

            __VISetSCL(1);

            if (wait4ClkHigh() == 0) {
                return 0;
            }

            __VISetSCL(0);
            data <<= 1;
        }

        __VISetSDA(1);
        __VISetSCL(1);

        if (wait4ClkHigh() == 0) {
            return 0;
        }

        if (nBytes != 1 && __VIGetSDA() != 0) {
            lastError = 1;
            return 0;
        }

        __VISetSCL(0);
        nBytes--;
    }

    __VISetSDA(0);
    __VISetSCL(1);
    __VISetSDA(1);
    return 1;
}
