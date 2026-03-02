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
    if (!context) return;
    context->mode  = 0;
    context->state = 0;
}

void OSInitContext(OSContext* context, u32 pc, u32 newsp) {
    if (!context) return;
    memset(context, 0, sizeof(OSContext));
    context->srr0 = pc;
    context->gpr[1] = newsp;
}

u32 OSSaveContext(OSContext* context) {
    // On PC we don't save PowerPC registers.
    // Return 0 = "context was just saved" (as opposed to 1 = "restored from save").
    return 0;
}

void OSLoadContext(OSContext* context) {
    // No-op on PC (no PowerPC register restore)
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
    // Return approximate stack pointer
    volatile u32 dummy;
    return (u32)(uintptr_t)&dummy;
}

u32 OSSwitchStack(u32 newsp) {
    // Not meaningful on PC - return current sp
    return OSGetStackPointer();
}

int OSSwitchFiber(u32 pc, u32 newsp) {
    // Not meaningful on PC
    OSReport("[PC] OSSwitchFiber: not supported on PC\n");
    return 0;
}

void __OSContextInit(void) {
    // On GC this installs the FPU exception handler.
    // On PC nothing to do.
}

#ifdef __cplusplus
}
#endif
