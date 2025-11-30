#include <revolution/os/OSIpc.h>
#include <revolution/os.h>

static void* IpcBufferHi;
static void* IpcBufferLo = (void*)0xFFFFFFFF;

void* __OSGetIPCBufferHi(void) {
    return IpcBufferHi;
}

void* __OSGetIPCBufferLo(void) {
    return IpcBufferLo;
}

void __OSInitIPCBuffer(void) {
    IpcBufferLo = (void*)*(u32*)OSPhysicalToCached(0x3100 + 0x0030);
    IpcBufferHi = (void*)*(u32*)OSPhysicalToCached(0x3100 + 0x0034);
} 
