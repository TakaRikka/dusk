#include <revolution/gd.h>
#include <revolution/os.h>

GDLObj* __GDCurrentDL = NULL;
static GDOverflowCb overflowcb = NULL;

void GDInitGDLObj(GDLObj* dl, void* start, u32 length) {
    ASSERTMSGLINE(49, !((u32)start & 0x1F), "start must be aligned to 32 bytes");
    ASSERTMSGLINE(50, !((u32)length & 0x1F), "length must be aligned to 32 bytes");
    dl->start = start;
    dl->ptr = start;
    dl->top = (u8*)start + length;
    dl->length = length;
}

void GDFlushCurrToMem(void) {
    DCFlushRange(__GDCurrentDL->start, __GDCurrentDL->length);
}

void GDPadCurr32(void) {
    u32 n;

    n = (u32)__GDCurrentDL->ptr & 0x1F;
    if (n != 0) {
        for (; n < 32; n = n + 1) {
            __GDWrite(0);
        }
    }
}

void GDOverflowed(void) {
    if (overflowcb) {
        (*overflowcb)();
    } else {
        ASSERTMSGLINE(86, 0, "GDWrite: display list overflowed");
    }
}

void GDSetOverflowCallback(GDOverflowCb callback) {
    overflowcb = callback;
}

GDOverflowCb GDGetOverflowCallback(void) {
    return overflowcb;
}
