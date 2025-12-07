#include <dolphin/exi.h>
#include <dolphin/hio.h>
#include <dolphin/os.h>
#include <dolphin/hw_regs.h>

static u32 Dev;
#define HIO_DEV Dev

#if DEBUG
const char* __HIOVersion = "<< Dolphin SDK - HIO\tdebug build: Apr  5 2004 03:57:05 (0x2301) >>";
#else
const char* __HIOVersion = "<< Dolphin SDK - HIO\trelease build: Apr  5 2004 04:15:47 (0x2301) >>";
#endif

static char __HIODigest1[71] = "\"HIO library - Copyright (C) 2000-2002 Nintendo. All rights reserved.\"";

#if DEBUG
static char __HIODigest2[34] = "\"HIO built: Apr  5 2004 03:57:05\"";
#else
static char __HIODigest2[34] = "\"HIO built: Apr  5 2004 04:15:47\"";
#endif

static s32 Chan = -1;
static HIOCallback ExiCallback;
static HIOCallback TxCallback;
static HIOCallback RxCallback;

static void ExtHandler(s32 chan, OSContext *context) {
    Chan = -1;
    HIO_DEV = 0;

    if (chan < 2 && HIO_DEV == 0) {
        EXISetExiCallback(chan, NULL);
    } else if (chan == 0 && HIO_DEV == 2) {
        EXISetExiCallback(2, NULL);
    }
}

static void ExiHandler(s32 chan, OSContext* context) {
    if (ExiCallback) {
        ExiCallback();
    }
}

static void DbgHandler(__OSInterrupt interrupt, OSContext *context) {
    OSContext exceptionContext;

    __PIRegs[0] = 0x1000;
    if (ExiCallback) {
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        ExiCallback();
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);
    }
}

static void TxHandler(s32 chan, OSContext* context) {
    EXIDeselect(Chan);
    EXIUnlock(Chan);
    if (TxCallback) {
        TxCallback();
    }
}

static void RxHandler(s32 chan, OSContext* context) {
    EXIDeselect(Chan);
    EXIUnlock(Chan);
    if (RxCallback) {
        RxCallback();
    }
}

BOOL HIOEnumDevices(HIOEnumCallback callback) {
    s32 chan;
    u32 id;

    if (Chan != -1 || callback == NULL) {
        return 0;
    }

    HIO_DEV = 0;
    for (chan = 0; chan <= 2; chan++) {
        if (chan < 2) {
            while (EXIProbeEx(chan) == 0) {}
        }

        if (EXIGetID(chan, HIO_DEV, &id) != 0 && id - 0x1010000 == 0 && !callback(chan)) {
            return 1;
        }
    }

    return 1;
}

static inline int CheckConsoleType() {
    if (__OSGetDIConfig() == 0xFF) {
        if (((OSGetConsoleType() & 0xF0000000) + 0xE0000000) == 0) {
            return 1;
        }

        switch (__OSDeviceCode) {
        case 0x8200:
            return 1;
        }

        return 0;
    }

    return 1;
}

BOOL HIOInit(s32 chan, HIOCallback callback) {
    int err;
    u32 cmd;
    u32 id;

    while (__OSDeviceCode == 0) {}

    if (CheckConsoleType() == 0) {
        Chan = -1;
        HIO_DEV = 0;
        OSReport("%s\n", __HIODigest1, __HIODigest2);
        return 0;
    }

    if (Chan != -1) {
        return 1;
    }

    Chan = chan;
    ExiCallback = callback;
    TxCallback = NULL;
    RxCallback = NULL;

    if (chan < 2 && HIO_DEV == 0) {
        while (EXIProbeEx(Chan) == 0) {}

        if (EXIAttach(Chan, ExtHandler) == 0) {
            Chan = -1;
            HIO_DEV = 0;
            return 0;
        }
    }

    if (EXILock(Chan, HIO_DEV, 0) == 0) {
        EXIDetach(Chan);
        Chan = -1;
        HIO_DEV = 0;
        return 0;
    }

    if (EXISelect(Chan, HIO_DEV, 0) == 0) {
        EXIUnlock(Chan);
        EXIDetach(Chan);
        Chan = -1;
        HIO_DEV = 0;
        return 0;
    }

    cmd = 0;
    err = 0;
    err |= !EXIImm(Chan, &cmd, 2, 1, 0);
    err |= !EXISync(Chan);
    err |= !EXIImm(Chan, &id, 4, 0, 0);
    err |= !EXISync(Chan);
    err |= !EXIDeselect(Chan);
    EXIUnlock(Chan);

    if (err != 0 || id != 0x1010000) {
        if (chan < 2 && HIO_DEV == 0) {
            EXIDetach(Chan);
        }
        EXIUnlock(Chan);
        Chan = -1;
        HIO_DEV = 0;
        return 0;
    }

    if (ExiCallback) {
        if (chan < 2) {
            if (HIO_DEV == 0) {
                EXISetExiCallback(Chan, ExiHandler);
            } else {
                ASSERTLINE(331, HIO_DEV == 2);
                EXISetExiCallback(2, ExiHandler);
            }
        } else {
            __OSSetInterruptHandler(0x19, DbgHandler);
            __OSUnmaskInterrupts(0x40);
        }
    }

    OSRegisterVersion(__HIOVersion);
    return 1;
}

BOOL HIOInitEx(s32 chan, u32 dev, HIOCallback callback) {
    ASSERTLINE(348, dev == 0 || chan == 0 && dev == 2);

    if (dev != 0 && (chan != 0 || dev != 2)) {
        return 0;
    }

    if (Chan != -1) {
        return 1;
    }

    HIO_DEV = dev;
    return HIOInit(chan, callback);
}

BOOL HIOReadMailbox(u32* word) {
    int err;
    u32 cmd;

    if (Chan == -1 || CheckConsoleType() == 0) {
        return 0;
    }

    if (EXILock(Chan, HIO_DEV, 0) == 0) {
        return 0;
    }

    if (EXISelect(Chan, HIO_DEV, 4) == 0) {
        EXIUnlock(Chan);
        return 0;
    }

    cmd = 0x60000000;
    err = 0;
    err |= !EXIImm(Chan, &cmd, 2, 1, 0);
    err |= !EXISync(Chan);
    err |= !EXIImm(Chan, word, 4, 0, 0);
    err |= !EXISync(Chan);
    err |= !EXIDeselect(Chan);
    EXIUnlock(Chan);
    return !err;
}

BOOL HIOWriteMailbox(u32 word) {
    int err;
    u32 cmd;

    if (Chan == -1 || CheckConsoleType() == 0) {
        return 0;
    }

    if (EXILock(Chan, HIO_DEV, 0) == 0) {
        return 0;
    }

    if (EXISelect(Chan, HIO_DEV, 4) == 0) {
        EXIUnlock(Chan);
        return 0;
    }

    cmd = (word & 0x1FFFFFFF) | 0xC0000000;
    err = 0;
    err |= !EXIImm(Chan, &cmd, 4, 1, 0);
    err |= !EXISync(Chan);
    err |= !EXIDeselect(Chan);
    EXIUnlock(Chan);
    return !err;
}

BOOL HIORead(u32 addr, void* buffer, s32 size) {
    int err;
    u32 cmd;

    if (Chan == -1 || CheckConsoleType() == 0) {
        return 0;
    }

    ASSERTLINE(436, (addr % 4) == 0);

    if (EXILock(Chan, HIO_DEV, 0) == 0) {
        return 0;
    }

    if (EXISelect(Chan, HIO_DEV, 4) == 0) {
        EXIUnlock(Chan);
        return 0;
    }

    cmd = ((addr << 8) & 0x01FFFC00) | 0x20000000;
    err = 0;
    err |= !EXIImm(Chan, &cmd, 4, 1, 0);
    err |= !EXISync(Chan);
    err |= !EXIDma(Chan, buffer, size, 0, NULL);
    err |= !EXISync(Chan);
    err |= !EXIDeselect(Chan);
    EXIUnlock(Chan);
    return !err;
}

BOOL HIOWrite(u32 addr, void* buffer, s32 size) {
    int err;
    u32 cmd;

    if (Chan == -1 || CheckConsoleType() == 0) {
        return 0;
    }

    ASSERTLINE(470, (addr % 4) == 0);

    if (EXILock(Chan, HIO_DEV, 0) == 0) {
        return 0;
    }

    if (EXISelect(Chan, HIO_DEV, 4) == 0) {
        EXIUnlock(Chan);
        return 0;
    }

    cmd = ((addr << 8) & 0x01FFFC00) | 0xA0000000;
    err = 0;
    err |= !EXIImm(Chan, &cmd, 4, 1, 0);
    err |= !EXISync(Chan);
    err |= !EXIDma(Chan, buffer, size, 1, NULL);
    err |= !EXISync(Chan);
    err |= !EXIDeselect(Chan);
    EXIUnlock(Chan);
    return !err;
}

BOOL HIOReadAsync(u32 addr, void* buffer, s32 size, HIOCallback callback) {
    int err;
    u32 cmd;

    if (Chan == -1 || CheckConsoleType() == 0) {
        return 0;
    }

    ASSERTLINE(504, (addr % 4) == 0);
    RxCallback = callback;

    if (EXILock(Chan, HIO_DEV, 0) == 0) {
        return 0;
    }

    if (EXISelect(Chan, HIO_DEV, 4) == 0) {
        EXIUnlock(Chan);
        return 0;
    }

    cmd = ((addr << 8) & 0x01FFFC00) | 0x20000000;
    err = 0;
    err |= !EXIImm(Chan, &cmd, 4, 1, 0);
    err |= !EXISync(Chan);
    err |= !EXIDma(Chan, buffer, size, 0, RxHandler);
    return !err;
}

BOOL HIOWriteAsync(u32 addr, void* buffer, s32 size, HIOCallback callback) {
    int err;
    u32 cmd;

    if (Chan == -1 || CheckConsoleType() == 0) {
        return 0;
    }

    ASSERTLINE(537, (addr % 4) == 0);
    TxCallback = callback;

    if (EXILock(Chan, HIO_DEV, 0) == 0) {
        return 0;
    }

    if (EXISelect(Chan, HIO_DEV, 4) == 0) {
        EXIUnlock(Chan);
        return 0;
    }

    cmd = ((addr << 8) & 0x01FFFC00) | 0xA0000000;
    err = 0;
    err |= !EXIImm(Chan, &cmd, 4, 1, 0);
    err |= !EXISync(Chan);
    err |= !EXIDma(Chan, buffer, size, 1, TxHandler);
    return !err;
}

BOOL HIOReadStatus(u32* status) {
    int err;
    u32 cmd;

    if (Chan == -1 || CheckConsoleType() == 0) {
        return 0;
    }

    if (EXILock(Chan, HIO_DEV, 0) == 0) {
        return 0;
    }

    if (EXISelect(Chan, HIO_DEV, 4) == 0) {
        EXIUnlock(Chan);
        return 0;
    }

    cmd = 0x40000000;
    err = 0;
    err |= !EXIImm(Chan, &cmd, 2, 1, 0);
    err |= !EXISync(Chan);
    err |= !EXIImm(Chan, status, 4, 0, 0);
    err |= !EXISync(Chan);
    err |= !EXIDeselect(Chan);
    EXIUnlock(Chan);
    return !err;
}
