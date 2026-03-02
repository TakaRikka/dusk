#include <dolphin/dolphin.h>
#include <dolphin/si.h>

#include "__si.h"

// prototypes
static void DefaultCallback(s32, s32);
static s32 SISteeringBegin(SISteeringControl* sc, SISteeringCallback callback);
static void ResetProc(s32 chan);
static int OnReset(BOOL final);

SISteeringControl __SISteering[4];
BOOL __SIResetSteering;

static OSResetFunctionInfo ResetFunctionInfo = {OnReset, 127};

void SIInitSteering(void) {
    static BOOL initialized;
    SISteeringControl* sc;
    s32 chan;

    if (!initialized) {
        initialized = TRUE;

        for (chan = 0; chan < 4; chan++) {
            sc = &__SISteering[chan];
            sc->ret = 0;
            OSInitThreadQueue(&sc->threadQueue);
        }

        SIRefreshSamplingRate();
        __SIResetSteering = 0;
        OSRegisterResetFunction(&ResetFunctionInfo);
    }
}

static void DefaultCallback(s32, s32) {}

static s32 SISteeringBegin(SISteeringControl* sc, SISteeringCallback callback) {
    BOOL enabled;
    s32 ret;

    callback = callback != 0 ? callback : DefaultCallback;

    enabled = OSDisableInterrupts();
    if (sc->callback) {
        ret = -2;
    } else {
        sc->callback = callback;
        ret = 0;
    }

    OSRestoreInterrupts(enabled);
    return ret;
}

static void ResetProc(s32 chan) {
    SISteeringControl* sc = &__SISteering[chan];

    if (sc->ret == 0 && __SIResetSteering == 0) {
        __SISteeringEnable(chan);
    }
}

s32 SIResetSteeringAsync(s32 chan, SISteeringCallback callback) {
    SISteeringControl* sc;
    s32 ret;

    sc = &__SISteering[chan];
    ret = SISteeringBegin(sc, callback);
    if (ret != 0) {
        return ret;
    }

    sc->output[0] = 0xFF;
    return __SISteeringTransfer(chan, 1, 3, ResetProc);
}

s32 SIResetSteering(s32 chan) {
    SISteeringControl* sc;
    s32 ret;

#if !DEBUG
    u32 padding;
#endif

    sc = &__SISteering[chan];
    ret = SIResetSteeringAsync(chan, __SISteeringSyncCallback);
    if (ret != 0) {
        return ret;
    }

    return __SISteeringSync(chan);
}

static int OnReset(BOOL final) {
    static s32 count;
    s32 chan;

    if (!__SIResetSteering) {
        __SIResetSteering = TRUE;

        for (chan = 0; chan < 4; chan++) {
            if ((0x80000000 >> chan) & __SISteeringEnableBits) {
                SIControlSteering(chan, 0x400, 0);
            }
        }

        count = VIGetRetraceCount();
    }

    if ((s32)VIGetRetraceCount() - count > 2) {
        while (__SISteeringEnableBits != 0) {
            __SISteeringDisable(__cntlzw(__SISteeringEnableBits));
        }
        return 1;
    }

    return 0;
}
