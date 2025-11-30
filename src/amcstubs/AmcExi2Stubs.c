/**
 * AmcExi2Stubs.c
 * Description:
 */

#include <dolphin/amc/AmcExi2Comm.h>

void EXI2_Init(vu8**, EXICallback) {}

void EXI2_EnableInterrupts(void) {}

int EXI2_Poll(void) {
    return 0;
}

AmcExiError EXI2_ReadN(void*, u32) {
    return 0;
}

AmcExiError EXI2_WriteN(const void*, u32) {
    return 0;
}

void EXI2_Reserve(void) {}

void EXI2_Unreserve(void) {}

BOOL AMC_IsStub(void) {
    return TRUE;
}
