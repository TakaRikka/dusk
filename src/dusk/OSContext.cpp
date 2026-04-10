// OSContext.cpp - PC implementation of GameCube OSContext API
// Replaces PowerPC assembly context switching with minimal PC stubs.
// On PC there is no register-level context save/restore; the OS handles
// thread contexts natively via std::thread.

#include <dolphin/dolphin.h>
#include <dolphin/os.h>
#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif

// --- Current context pointer (per-process, not per-thread) ---
static OSContext* sCurrentContext = nullptr;

OSContext* OSGetCurrentContext(void) {
    return sCurrentContext;
}

void OSSetCurrentContext(OSContext* context) {
    sCurrentContext = context;
}

void OSClearContext(OSContext* context) {
    // No-op on PC
}

void OSInitContext(OSContext* context, u32 pc, u32 newsp) {
    // No-op on PC
}

void OSDumpContext(OSContext* context) {
    if (!context) {
        OSReport("[PC] OSDumpContext: NULL context\n");
        return;
    }
    OSReport("[PC] OSDumpContext: context at %p (no register info on PC)\n", context);
}

void OSFillFPUContext(OSContext* context) {
    // No-op on PC (no PowerPC FPU state)
}

void OSLoadFPUContext(OSContext* fpucontext) {
    // No-op on PC
}

void OSSaveFPUContext(OSContext* fpucontext) {
    // No-op on PC
}

u32 OSGetStackPointer(void) {
    return 0;
}

u32 OSSwitchStack(u32 newsp) {
    abort();
}

int OSSwitchFiber(u32 pc, u32 newsp) {
    abort();
}

void __OSContextInit(void) {
    // On GC this installs the FPU exception handler.
    // On PC nothing to do.
}

#ifdef __cplusplus
}
#endif
