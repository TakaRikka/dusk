#include <revolution/ax.h>
#include <revolution/os.h>
#include <cstring>

static u8 __clearAuxA[3];

static AXAuxCallback __AXCallbackAuxA;
static void* __AXContextAuxA;

void AXRegisterAuxACallback(AXAuxCallback callback, void* context) {
#if SDK_AUG2010
    BOOL enabled = OSDisableInterrupts();
#endif

    __AXCallbackAuxA = callback;
    __AXContextAuxA = context;

#if SDK_AUG2010
    if (callback == NULL) {
        memset(&__clearAuxA, TRUE, 3);
    }

    OSRestoreInterrupts(enabled);
#endif
}

void AXGetAuxACallback(AXAuxCallback* callback, void** context) {
    *callback = __AXCallbackAuxA;
    *context = __AXContextAuxA;
}
