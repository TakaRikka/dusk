#include <dolphin.h>
#include <dolphin/ax.h>

#include "__ax.h"

#if DEBUG
const char* __AXVersion = "<< Dolphin SDK - AX\tdebug build: Apr  5 2004 03:56:21 (0x2301) >>";
#else
const char* __AXVersion = "<< Dolphin SDK - AX\trelease build: Apr  5 2004 04:15:05 (0x2301) >>";
#endif

void AXInit(void) {
    AXInitEx(0);
}

void AXInitEx(u32 outputBufferMode) {
#if DEBUG
    OSReport("Initializing AX\n");
#endif
    OSRegisterVersion(__AXVersion);

    __AXAllocInit();
    __AXVPBInit();
    __AXSPBInit();
    __AXAuxInit();
    __AXClInit();
    __AXOutInit(outputBufferMode);
}

void AXQuit(void) {
#if DEBUG
    OSReport("Shutting down AX\n");
#endif
    __AXAllocQuit();
    __AXVPBQuit();
    __AXSPBQuit();
    __AXAuxQuit();
    __AXClQuit();
    __AXOutQuit();
}
