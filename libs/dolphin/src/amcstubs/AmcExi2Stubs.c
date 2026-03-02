#include <dolphin/dolphin.h>
#include <dolphin/amc/AmcExi2Comm.h>

// prototypes
int AMC_IsStub(void);

void EXI2_Init(volatile unsigned char **inputPendingPtrRef, EXICallback monitorCallback) {}

void EXI2_EnableInterrupts(void) {}

int EXI2_Poll(void) {
    return 0;
}

AmcExiError EXI2_ReadN(void *bytes, unsigned long length) {
    return AMC_EXI_NO_ERROR;
}

AmcExiError EXI2_WriteN(const void *bytes, unsigned long length) { 
    return AMC_EXI_NO_ERROR;
}

void EXI2_Reserve(void) {}

void EXI2_Unreserve(void) {}

int AMC_IsStub(void) {
    return 1;
}
