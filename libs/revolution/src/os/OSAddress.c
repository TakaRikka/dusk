#include <revolution/os.h>

// undefine the macros so they do not error the file.
#undef OSPhysicalToCached
#undef OSPhysicalToUncached
#undef OSCachedToPhysical
#undef OSUncachedToPhysical
#undef OSCachedToUncached
#undef OSUncachedToCached

void* OSPhysicalToCached(u32 paddr) {
    ASSERTMSGLINE(56, paddr < 0x20000000U, "OSPhysicalToCached(): illegal address.");
    return (void*)(paddr + 0x80000000);
}

void* OSPhysicalToUncached(u32 paddr) {
    ASSERTMSGLINE(71, paddr < 0x20000000U, "OSPhysicalToUncached(): illegal address.");
    return (void*)(paddr - 0x40000000);
}

u32 OSCachedToPhysical(void* caddr) {
    ASSERTMSGLINE(86, 0x80000000U <= (u32)caddr && (u32)caddr < 0xA0000000U, "OSCachedToPhysical(): illegal address.");
    return (u32)caddr + 0x80000000;
}

u32 OSUncachedToPhysical(void* ucaddr) {
    ASSERTMSGLINE(101, 0xC0000000U <= (u32)ucaddr && (u32)ucaddr < 0xE0000000U, "OSUncachedToPhysical(): illegal address.");
    return (u32)ucaddr + 0x40000000;
}

void* OSCachedToUncached(void* caddr) {
    ASSERTMSGLINE(116, 0x80000000U <= (u32)caddr && (u32)caddr < 0xA0000000U, "OSCachedToUncached(): illegal address.");
    return (void*)((u32)caddr + 0x40000000);
}
