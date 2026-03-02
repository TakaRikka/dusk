#include <revolution/exi.h>
#include <revolution/os.h>

#include "__os.h"

static SramControl Scb ATTRIBUTE_ALIGN(DOLPHIN_ALIGNMENT);

// prototypes
static int GetRTC(u32* rtc);
static int ReadSram(void* buffer);
static void WriteSramCallback(s32, OSContext*);
static int WriteSram(void* buffer, u32 offset, u32 size);
static void* LockSram(u32 offset);
static int UnlockSram(int commit, u32 offset);
static void __OSReadROMCallback(s32 chan);

static int GetRTC(u32* rtc) {
    int err;
    u32 cmd;

    if (EXILock(0, 1, NULL) == 0) {
        return 0;
    }
    if (EXISelect(0, 1, 3) == 0) {
        EXIUnlock(0);
        return 0;
    }
    cmd = 0x20000000;
    err = 0;
    err |= !EXIImm(0, &cmd, 4, 1, 0);
    err |= !EXISync(0);
    err |= !EXIImm(0, &cmd, 4, 0, 0);
    err |= !EXISync(0);
    err |= !EXIDeselect(0);
    EXIUnlock(0);
    rtc[0] = cmd;
    return !err;
}

int __OSGetRTC(u32* rtc) {
    int err;
    u32 t0;
    u32 t1;
    int i;

    for(i = 0; i < 16; i++) {
        err = 0;
        err |= !GetRTC(&t0);
        err |= !GetRTC(&t1);
        if (err) {
            break;
        }
        if (t0 == t1) {
            rtc[0] = t0;
            return 1;
        }
    }
    return 0;
}

int __OSSetRTC(u32 rtc) {
    int err;
    u32 cmd;

    if (EXILock(0, 1, NULL) == 0) {
        return 0;
    }
    if (EXISelect(0, 1, 3) == 0) {
        EXIUnlock(0);
        return 0;
    }
    cmd = 0xA0000000;
    err = 0;
    err |= !EXIImm(0, &cmd, 4, 1, 0);
    err |= !EXISync(0);
    err |= !EXIImm(0, &rtc, 4, 1, 0);
    err |= !EXISync(0);
    err |= !EXIDeselect(0);
    EXIUnlock(0);
    return !err;
}

static int ReadSram(void* buffer) {
    int err;
    u32 cmd;

    DCInvalidateRange(buffer, SRAM_SIZE);
    if (!EXILock(0, 1, NULL) ) {
        return 0;
    }
    if (!EXISelect(0, 1, 3)) {
        EXIUnlock(0);
        return 0;
    }
    cmd = 0x20000100;
    err = 0;
    err |= !EXIImm(0, &cmd, 4, 1, 0);
    err |= !EXISync(0);
    err |= !EXIDma(0, buffer, SRAM_SIZE, 0, NULL);
    err |= !EXISync(0);
    err |= !EXIDeselect(0);
    EXIUnlock(0);
    return !err;
}

static void WriteSramCallback(s32, OSContext*) {
    ASSERTLINE(270, !Scb.locked);
    Scb.sync = WriteSram(&Scb.sram[Scb.offset], Scb.offset, SRAM_SIZE - Scb.offset);
    if (Scb.sync != 0) {
        Scb.offset = SRAM_SIZE;
    }
    ASSERTLINE(276, Scb.sync);
}

static int WriteSram(void* buffer, u32 offset, u32 size) {
    int err;
    u32 cmd;

    if (!EXILock(0, 1, WriteSramCallback)) {
        return 0;
    }
    if (!EXISelect(0, 1, 3)) {
        EXIUnlock(0);
        return 0;
    }
    offset <<= 6;
    cmd = ((offset + 0x100) | 0xA0000000);
    err = 0;
    err |= !EXIImm(0, &cmd, 4, 1, 0);
    err |= !EXISync(0);
    err |= !EXIImmEx(0, buffer, size, 1);
    err |= !EXIDeselect(0);
    EXIUnlock(0);
    return !err;
}

void __OSInitSram(void) {
    Scb.locked = Scb.enabled = FALSE;
    Scb.sync = ReadSram(&Scb);
    ASSERTLINE(330, Scb.sync);
    Scb.offset = SRAM_SIZE;

    OSSetGbsMode(OSGetGbsMode());
}

static void* LockSram(u32 offset) {
    BOOL enabled;

    enabled = OSDisableInterrupts();
    ASSERTLINE(353, !Scb.locked);
    if (Scb.locked) {
        OSRestoreInterrupts(enabled);
        return NULL;
    }
    Scb.enabled = enabled;
    Scb.locked = TRUE;
    return &Scb.sram[offset];
}

OSSram* __OSLockSram(void) {
    return (OSSram*)LockSram(0);
}

OSSramEx* __OSLockSramEx(void) {
    return (OSSramEx*)LockSram(sizeof(OSSram));
}

static int UnlockSram(int commit, u32 offset) {
    u16* p;

    ASSERTLINE(387, Scb.locked);
    if (commit != 0) {
        if (offset == 0) {
            OSSram* sram  = (OSSram*)Scb.sram;
            if (2u < (sram->flags & 3)) {
                sram->flags &= ~3;
            }

            sram->checkSum = sram->checkSumInv = 0;
            for(p = (u16*)&sram->counterBias; p < ((u16*)&Scb.sram[0x14]); p++) {
                sram->checkSum += *p;
                sram->checkSumInv += ~(*p);
            }
        }
        if (offset < Scb.offset) {
            Scb.offset = offset;
        }

        if (Scb.offset <= 0x14) {
            OSSramEx* sram = (OSSramEx*)(Scb.sram + sizeof(OSSram));
            if (((u32)sram->gbs & 0x7c00) == 0x5000 || ((u32)sram->gbs & 0xc0) == 0xc0) {
                sram->gbs = 0;
            }
        }

        Scb.sync = WriteSram(&Scb.sram[Scb.offset], Scb.offset, SRAM_SIZE - Scb.offset);
        if (Scb.sync != 0) {
            Scb.offset = SRAM_SIZE;
        }
    }
    Scb.locked = FALSE;
    OSRestoreInterrupts(Scb.enabled);
    return Scb.sync;
}

int __OSUnlockSram(int commit) {
    UnlockSram(commit, 0);
}

int __OSUnlockSramEx(int commit) {
    UnlockSram(commit, sizeof(OSSram));
}

int __OSSyncSram(void) {
    return Scb.sync;
}

int __OSCheckSram(void) {
    u16* p;
    u16 checkSum;
    u16 checkSumInv;
    OSSram* sram;
    int unused;

    ASSERTLINE(466, Scb.locked);

    checkSum = checkSumInv = 0;

    sram = (OSSram*)Scb.sram;

    for (p = (void*)&sram->counterBias; p < (u16*)&Scb.sram[0x14]; p++) {
        checkSum += *p;
        checkSumInv += ~(*p);
    }

    return (sram->checkSum == checkSum && sram->checkSumInv == checkSumInv);
}

int __OSReadROM(void * buffer, s32 length, s32 offset) {
    int err;
    u32 cmd;

    ASSERTLINE(509, length <= 1024);
    DCInvalidateRange(buffer, length);
    if (EXILock(0, 1, NULL) == 0) {
        return 0;
    }
    if (EXISelect(0, 1, 3) == 0) {
        EXIUnlock(0);
        return 0;
    }
    cmd = offset << 6;
    err = 0;
    err |= !EXIImm(0, &cmd, 4, 1, 0);
    err |= !EXISync(0);
    err |= !EXIDma(0, buffer, length, 0, NULL);
    err |= !EXISync(0);
    err |= !EXIDeselect(0);
    EXIUnlock(0);
    return !err;
}

static void __OSReadROMCallback(s32 chan) {
    void (*callback)();

    EXIDeselect(chan);
    EXIUnlock(chan);
    callback = Scb.callback;
    if (callback) {
        Scb.callback = NULL;
        callback();
    }
}

int __OSReadROMAsync(void* buffer, s32 length, s32 offset, void (*callback)()) {
    int err;
    u32 cmd;

    ASSERTLINE(556, length <= 1024);
    ASSERTLINE(557, callback);
    DCInvalidateRange(buffer, length);
    Scb.callback = callback;
    if (EXILock(0, 1, NULL) == 0) {
        return 0;
    }
    if (EXISelect(0, 1, 3) == 0) {
        EXIUnlock(0);
        return 0;
    }
    cmd = offset << 6;
    err = 0;
    err |= !EXIImm(0, &cmd, 4, 1, 0);
    err |= !EXISync(0);
    err |= !EXIDma(0, buffer, length, 0, (void*)__OSReadROMCallback);
    return !err;
}

u32 OSGetSoundMode(void) {
    OSSram* sram = __OSLockSram();
    u32 mode = (sram->flags & 4) ? 1 : 0;

    __OSUnlockSram(0);
    return mode;
}

void OSSetSoundMode(u32 mode) {
    OSSram* sram;
    int unused;

    ASSERTLINE(617, mode == OS_SOUND_MODE_MONO || mode == OS_SOUND_MODE_STEREO);
    mode *= 4;
    mode &= 4;
    sram = __OSLockSram();
    if (mode == (sram->flags & 4)) {
        __OSUnlockSram(0);
        return;
    }
    sram->flags &= 0xFFFFFFFB;
    sram->flags |= mode;
    __OSUnlockSram(1);
}

u32 OSGetProgressiveMode(void) {
    OSSram* sram;
    u32 on;

    sram = __OSLockSram();
    on = (sram->flags & 0x80) >> 7;
    __OSUnlockSram(FALSE);
    return on;
}

void OSSetProgressiveMode(u32 on) {
#if !DEBUG
    u32 padding[1];
#endif
    OSSram* sram;

    ASSERTLINE(670, on == OS_PROGRESSIVE_MODE_OFF || on == OS_PROGRESSIVE_MODE_ON);

    on <<= 7;
    on &= 0x80;

    sram = __OSLockSram();
    if (on == (sram->flags & 0x80)) {
        __OSUnlockSram(FALSE);
        return;
    }

    sram->flags &= ~0x80;
    sram->flags |= on;
    __OSUnlockSram(TRUE);
}

u32 OSGetVideoMode(void) {
    OSSram* sram = __OSLockSram();
    u32 mode = sram->flags & 3;

    __OSUnlockSram(0);

    if (mode > 2) {
        mode = 0;
    }

    return mode;
}

void OSSetVideoMode(u32 mode) {
    OSSram* sram;
    int unused;

    ASSERTLINE(731, OS_VIDEO_MODE_NTSC <= mode && mode <= OS_VIDEO_MODE_MPAL);

    if (mode > 2) {
        mode = 0;
    }

    sram = __OSLockSram();

    if (mode == (sram->flags & 3)) {
        __OSUnlockSram(0);
        return;
    }
    sram->flags &= 0xFFFFFFFC;
    sram->flags |= mode;
    __OSUnlockSram(1);
}

u8 OSGetLanguage(void) {
    OSSram* sram = __OSLockSram();
    u8 language = sram->language;

    __OSUnlockSram(0);
    return language;
}

void OSSetLanguage(u8 language) {
    OSSram* sram = __OSLockSram();
    int unused;

    if (language == sram->language) {
        __OSUnlockSram(0);
        return;
    }
    sram->language = language;
    __OSUnlockSram(1);
}

u8 __OSGetBootMode(void) {
    OSSram* sram = __OSLockSram();
    u8 ntd = sram->ntd;
    __OSUnlockSram(0);
    return ntd & 0x80;
}

void __OSSetBootMode(u8 ntd) {
    OSSram* sram;
    int unused;

    ntd &= 0x80;
    sram = __OSLockSram();
    if (ntd == (sram->ntd & 0x80U)) {
        __OSUnlockSram(0);
        return;
    }
    sram->ntd &= 0xFFFFFF7F;
    sram->ntd |= ntd;
    __OSUnlockSram(1);
}

u32 OSGetEuRgb60Mode(void) {
    OSSram* sram;
    u32 on;

    sram = __OSLockSram();
    on = (sram->ntd & 0x40) >> 6;
    __OSUnlockSram(0);
    return on;
}

void OSSetEuRgb60Mode(u32 on) {
#if !DEBUG
    u32 padding[1];
#endif
    OSSram* sram;

    ASSERTLINE(895, on == OS_EURGB60_OFF || on == OS_EURGB60_ON);
    on <<= 6;
    on &= 0x40;

    sram = __OSLockSram();
    if (on == (sram->ntd & 0x40)) {
        __OSUnlockSram(0);
    } else {
        sram->ntd &= ~0x40;
        sram->ntd |= on;
        __OSUnlockSram(1);
    }
}

u16 OSGetWirelessID(s32 chan) {
    OSSramEx* sram;
    u16 id;

    sram = __OSLockSramEx();
    id = sram->wirelessPadID[chan];
    __OSUnlockSramEx(FALSE);
    return id;
}

void OSSetWirelessID(s32 chan, u16 id) {
    OSSramEx* sram;

    sram = __OSLockSramEx();
    if (sram->wirelessPadID[chan] != id) {
        sram->wirelessPadID[chan] = id;
        __OSUnlockSramEx(TRUE);
        return;
    }

    __OSUnlockSramEx(FALSE);
}

u16 OSGetGbsMode(void) {
    OSSramEx* sram;
    u16 mode;

    sram = __OSLockSramEx();
    mode = sram->gbs;
    __OSUnlockSramEx(FALSE);
    return mode;
}

void OSSetGbsMode(u16 mode) {
#if !DEBUG
    u32 padding[1];
#endif
    OSSramEx* sram;

    if (((u32)mode & 0x7c00) == 0x5000 || ((u32)mode & 0xc0) == 0xc0) {
        mode = 0;
    }

    sram = __OSLockSramEx();

    if (mode == sram->gbs) {
        __OSUnlockSramEx(FALSE);
        return;
    }
    sram->gbs = mode;

    __OSUnlockSramEx(TRUE);
}

BOOL __OSGetRTCFlags(u32* flags) {
    BOOL err;
    u32 cmd;

    if (!EXILock(0, 1, 0)) {
        return FALSE;
    }

    if (!EXISelect(0, 1, 3)) {
        EXIUnlock(0);
        return FALSE;
    }

    cmd = 0x21000800;
    err = FALSE;
    err |= !EXIImm(0, &cmd, 4, 1, NULL);
    err |= !EXISync(0);
    err |= !EXIImm(0, &cmd, 4, 0, NULL);
    err |= !EXISync(0);
    err |= !EXIDeselect(0);
    EXIUnlock(0);
    *flags = cmd;
    return !err;
}

BOOL __OSClearRTCFlags(void) {
    BOOL err;
    u32 cmd;
    u32 data = 0;

    if (!EXILock(0, 1, 0)) {
        return FALSE;
    }

    if (!EXISelect(0, 1, 3)) {
        EXIUnlock(0);
        return FALSE;
    }
    
    cmd = 0xA1000800;
    err = FALSE;
    err |= !EXIImm(0, &cmd, 4, 1, NULL);
    err |= !EXISync(0);
    err |= !EXIImm(0, &data, 4, 1, NULL);
    err |= !EXISync(0);
    err |= !EXIDeselect(0);
    EXIUnlock(0);

    return !err;
}
