#include <revolution/types.h>
#include <revolution/os.h>
#include <revolution/exi.h>
#include <revolution/hio2.h>
#include <revolution/hw_regs.h>

#ifdef SDK_AUG2010
#define BUILD_DATE "Aug 23 2010"
#if DEBUG
#define BUILD_TIME "17:28:30"
#else
#define BUILD_TIME "17:33:06"
#endif
#elif SDK_SEP2006
#define BUILD_DATE  "Sep 21 2006"
#define BUILD_TIME "14:32:13"
#endif

#ifdef SDK_AUG2010
#if DEBUG
const char* __HIO2Version = "<< RVL_SDK - HIO2 \tdebug build: "BUILD_DATE" "BUILD_TIME" (0x4302_145) >>";
#else
const char* __HIO2Version = "<< RVL_SDK - HIO2 \trelease build: "BUILD_DATE" "BUILD_TIME" (0x4302_145) >>";
#endif
#elif SDK_SEP2006
const char* __HIO2Version = "<< RVL_SDK - HIO2 \trelease build: "BUILD_DATE" "BUILD_TIME" (0x4200_60422) >>";
#endif

HIO2Control __HIO2Control[2] = {
    {HIO2_DEVICE_INVALID, -1, 0, NULL, NULL, NULL, NULL},
    {HIO2_DEVICE_INVALID, -1, 0, NULL, NULL, NULL, NULL},
};

static u32 __HIO2ConsoleType;
static BOOL __HIO2Initialized;
static int __HIO2LastErrorCode;

static BOOL __HIO2IsInitialized();

static void __HIO2ClearChanInfo(s32 chan) {
    __HIO2Control[chan].type = HIO2_DEVICE_INVALID;
    __HIO2Control[chan].chan = -1;
    __HIO2Control[chan]._0x8 = 0;

    __HIO2Control[chan].exiCallback =
    __HIO2Control[chan]._0x10       =
    __HIO2Control[chan]._0x14       =
    __HIO2Control[chan].disconnectCb = NULL;
}

static void __HIO2ExtHandler(s32 chan, OSContext* context) {
    if (__HIO2Control[chan].disconnectCb != NULL) {
        __HIO2Control[chan].disconnectCb(chan);
    }

    __HIO2ClearChanInfo(chan);
    EXISetExiCallback(chan, NULL);
}

static void __HIO2ExiHandler(s32 chan, OSContext* context) {
    if (chan == 2) {
        chan = 0;
    }

    if (__HIO2Control[chan].exiCallback != NULL) {
        __HIO2Control[chan].exiCallback(chan);
    }
}

BOOL HIO2Init(void) {
    if (__HIO2Initialized) {
        HIO2Exit();
    }

    EXIWait();
    __HIO2ConsoleType = EXIGetConsoleType();

    if (!(__HIO2ConsoleType & 7)) {
        __HIO2LastErrorCode = 6;
        return FALSE;
    }

    __HIO2LastErrorCode = 0;
    OSRegisterVersion(__HIO2Version);
    return __HIO2Initialized = TRUE;
}

BOOL HIO2EnumDevices(HIO2EnumCallback callback) {
    s32 chan;
    u32 id;

    ASSERTLINE(275, callback != NULL);

    if (!__HIO2IsInitialized()) {
        return FALSE;
    }

    if (__HIO2Control[0].chan != -1 && __HIO2Control[1].chan != -1) {
        __HIO2LastErrorCode = 5;
        return FALSE;
    }

    for (chan = 0; chan < 2; chan++) {
        while (EXIProbeEx(chan) == 0) {}

        if (EXIGetID(chan, 0, &id) != 0 && id == 0 && !callback(chan)) {
            return TRUE;
        }
    }

    if (__HIO2ConsoleType & 1) {
        callback(2);
    }

    return TRUE;
}

static BOOL __HIO2IsInitialized(void) {
    if (!__HIO2Initialized) {
        __HIO2LastErrorCode = 1;
    }

    return __HIO2Initialized;
}

s32 HIO2Open(HIO2DeviceType type, HIO2UnkCallback exiCb, HIO2DisconnectCallback disconnectCb) {
    s32 chan;
    s32 device;
    BOOL enabled;
    u8 reg;
    
    if (!__HIO2IsInitialized()) {
        return -1;
    }

    switch (type) {
    case HIO2_DEVICE_UNK_0:
    case HIO2_DEVICE_UNK_1:
        chan = type;
        device = 0;
        break;
    case HIO2_DEVICE_UNK_2:
        if (__HIO2ConsoleType & 1) {
            chan = 0;
            device = 1;
        } else {
            __HIO2LastErrorCode = 6;
            return -1;
        }
        break;
    default:
        __HIO2LastErrorCode = 3;
        return -1;
    }

    if (__HIO2Control[chan].chan != -1) {
        __HIO2LastErrorCode = 4;
        return -1;
    }

    if (device == 0) {
        u32 id = 0;
        while (EXIProbeEx(chan) == 0) {}

        if (!EXIAttach(chan, __HIO2ExtHandler) || EXIGetID(chan, 0, &id) == 0 || id != 0) {
            __HIO2LastErrorCode = 7;
            return -1;
        }
    }

    reg = 0xD8;
    enabled = OSDisableInterrupts();

    if (!EXIWriteReg(chan, device, 0xB4000000, &reg, 1)) {
        if (device == 0) {
            EXIDetach(chan);
        }

        __HIO2LastErrorCode = 8;
        OSRestoreInterrupts(enabled);
        return -1;
    }

    OSRestoreInterrupts(enabled);

    __HIO2Control[chan].type = type;
    __HIO2Control[chan]._0x8 = device;
    __HIO2Control[chan].chan = chan;
    __HIO2Control[chan].exiCallback = exiCb;
    __HIO2Control[chan]._0x10 = NULL;
    __HIO2Control[chan]._0x14 = NULL;
    __HIO2Control[chan].disconnectCb = disconnectCb;

    if (exiCb != NULL) {
        if (type == HIO2_DEVICE_UNK_2) {
            EXISetExiCallback(2, __HIO2ExiHandler);
        } else {
            EXISetExiCallback(chan, __HIO2ExiHandler);
        }
    }

    return chan;
}

BOOL __HIO2IsValidHandle(s32 handle) {
    if ((handle == 0 || handle == 1) && __HIO2Control[handle].chan != -1) {
        return TRUE;
    }

    __HIO2LastErrorCode = 2;
    return FALSE;
}

BOOL HIO2Close(s32 handle) {
    if (!__HIO2IsInitialized() || !__HIO2IsValidHandle(handle)) {
        return FALSE;
    }

    EXISetExiCallback(__HIO2Control[handle].chan, NULL);

    if (__HIO2Control[handle]._0x8 == 0) {
        EXIDetach(__HIO2Control[handle].chan);
    }

    __HIO2ClearChanInfo(handle);
    return TRUE;
}

BOOL HIO2Read(s32 handle, u32 addr, void* buffer, s32 size) {
    BOOL rt;

    if (!__HIO2IsInitialized() || !__HIO2IsValidHandle(handle)) {
        return FALSE;
    }

    ASSERTLINE(564, (addr & 3) == 0);

    rt = EXIReadRam(handle, __HIO2Control[handle]._0x8, ((addr + 0xD10000) << 6) & 0x3FFFFF00, buffer, size, NULL);
    if (rt == 0) {
        __HIO2LastErrorCode = 8;
    }

    return rt;
}

BOOL HIO2Write(s32 handle, u32 addr, void* buffer, s32 size) {
    BOOL rt;

    if (!__HIO2IsInitialized() || !__HIO2IsValidHandle(handle)) {
        return FALSE;
    }

    ASSERTLINE(593, (addr & 3) == 0);

    rt = EXIWriteRam(handle, __HIO2Control[handle]._0x8, (((addr + 0xD10000) << 6) & 0x3FFFFF00) | 0x80000000, buffer, size, NULL);
    if (rt == 0) {
        __HIO2LastErrorCode = 8;
    }

    return rt;
}

void HIO2Exit(void) {
    s32 handle;
    for (handle = 0; handle < 2; handle++) {
        if (__HIO2Control[handle].chan != -1) {
            HIO2Close(handle);
        }
    }

    __HIO2Initialized = FALSE;
    __HIO2ConsoleType = 0;
}
