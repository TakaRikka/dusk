#include <revolution/types.h>
#include <revolution/os.h>
#include <revolution/exi.h>

const u32 __EXIFreq = 4;

s32 EXIGetConsoleType(void) {
    return 1;
}

void EXIWait(void) {
    while (__OSDeviceCode == 0) {}
}

static u32 __EXISwap32(u32 val) {
    return val >> 24 & 0x000000FF | val >> 8 & 0x0000FF00 | val << 8 & 0x00FF0000 | val << 24 & 0xFF000000;
}

BOOL EXIWriteReg(s32 chan, u32 dev, u32 exiCmd, void* reg, s32 size) {
    BOOL ret = FALSE;
    u32 reg32;

    switch (size) {
    case 1:
        reg32 = (u32)((*((u8*)reg) & 0xFF) << 24);
        break;
    case 2:
        reg32 = (u32)(((*((u16*)reg) & 0xFF) << 24) | ((*((u16*)reg) & 0xFF00) << 8));
        break;
    default:
        reg32 = __EXISwap32(*((u32*)reg));
        break;
    }

    ret |= !EXILock(chan, dev, 0);

    if (ret) {
        return FALSE;
    }

    ret |= !EXISelect(chan, dev, 4);

    if (ret) {
        EXIUnlock(chan);
        return FALSE;
    }

    ret |= !EXIImm(chan, &exiCmd, 4, 1, NULL);
    ret |= !EXISync(chan);
    ret |= !EXIImm(chan, &reg32, 4, 1, NULL);
    ret |= !EXISync(chan);
    ret |= !EXIDeselect(chan);
    ret |= !EXIUnlock(chan);
    return !ret;
}

BOOL EXIReadRam(s32 chan, u32 dev, u32 exiCmd, void* buffer, s32 size, EXICallback callback) {
    BOOL ret = FALSE;

    ret |= !EXILock(chan, dev, NULL);

    if (ret) {
        return FALSE;
    }

    ret |= !EXISelect(chan, dev, 4);

    if (ret) {
        EXIUnlock(chan);
        return FALSE;
    }

    ret |= !EXIImm(chan, &exiCmd, 4, 1, NULL);
    ret |= !EXISync(chan);
    ret |= !EXIDma(chan, buffer, size, EXI_READ, callback);

    if (callback == NULL) {
        ret |= !EXISync(chan);
        ret |= !EXIDeselect(chan);
        ret |= !EXIUnlock(chan);
    }
    
    return !ret;
}

BOOL EXIWriteRam(s32 chan, u32 dev, u32 exiCmd, void* buffer, s32 size, EXICallback callback) {
    BOOL ret = FALSE;

    ret |= !EXILock(chan, dev, NULL);

    if (ret) {
        return FALSE;
    }

    ret |= !EXISelect(chan, dev, 4);

    if (ret) {
        EXIUnlock(chan);
        return FALSE;
    }

    ret |= !EXIImm(chan, &exiCmd, 4, 1, NULL);
    ret |= !EXISync(chan);
    ret |= !EXIDma(chan, buffer, size, EXI_WRITE, callback);

    if (callback == NULL) {
        ret |= !EXISync(chan);
        ret |= !EXIDeselect(chan);
        ret |= !EXIUnlock(chan);
    }
    
    return !ret;
}
