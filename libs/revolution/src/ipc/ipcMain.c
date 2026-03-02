#include <revolution/ipc.h>
#include <revolution/os/OSIpc.h>
#include <revolution/os.h>

static void* IPCBufferHi;
static void* IPCBufferLo;

static void* IPCCurrentBufferHi;
static void* IPCCurrentBufferLo;

static u8 Initialized;

void IPCInit(void) {
    if (Initialized) {
        return;
    }

    IPCBufferHi = __OSGetIPCBufferHi();
    IPCBufferLo = __OSGetIPCBufferLo();
    IPCCurrentBufferHi = IPCBufferHi;
    IPCCurrentBufferLo = IPCBufferLo;

    Initialized = TRUE;
}

#if SDK_AUG2010
void IPCReInit(void) {
    Initialized = FALSE;
    IPCInit();
}
#endif

u32 IPCReadReg(u32 regIdx) {
    u32 reg = __IPCRegs[regIdx];
    return reg;
}

void IPCWriteReg(u32 regIdx, u32 data) {
    __IPCRegs[regIdx] = data;
}

void* IPCGetBufferHi(void) {
    return IPCCurrentBufferHi;
}

void* IPCGetBufferLo(void) {
    return IPCCurrentBufferLo;
}

void IPCSetBufferLo(void* newLo) {
    ASSERTLINE(296, IPCBufferLo <= newLo);
    IPCCurrentBufferLo = newLo;
}
