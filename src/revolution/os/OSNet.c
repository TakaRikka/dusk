#include <revolution/os.h>
#include <revolution/sc.h>
#include <revolution/private/iosresclt.h>

static OSShutdownFunctionInfo ShutdownFuncInfo;

static IOSFd nwc24ShtFd = -1;
static int nwc24ShtRetryRest = 0;
static BOOL NWC24iIsRequestPending = FALSE;

typedef enum NWC24Err {
    NWC24_OK = 0,
} NWC24Err;

NWC24Err NWC24iPrepareShutdown(void);
NWC24Err NWC24iSynchronizeRtcCounter(BOOL);
NWC24Err NWC24iOpenResourceManager_(const char* callerName, const char* path, IOSFd* fd, u32 flags);
NWC24Err NWC24SuspendScheduler(void);
NWC24Err NWC24iSetRtcCounter_(int rtc, BOOL);
NWC24Err NWC24iIoctlResourceManager_(const char* callerName, IOSFd fd, s32 cmd, void* input, u32 inputLen, void* output, u32 outputLen);
NWC24Err NWC24iCloseResourceManager_(const char* callerName, IOSFd);
NWC24Err NWC24iRequestShutdown(u32 event, s32* result);
NWC24Err NWC24iIoctlResourceManagerAsync_(const char* callerName, IOSFd fd, s32 cmd, void* input, u32 inputLen, void* output, u32 outputLen, void* cbArg);
BOOL NWC24iIsAsyncRequestPending_(void);

BOOL NWC24Shutdown_(BOOL, u32);

static void REXInit(void);
static NWC24Err GetRTC(int* rtc);
static s32 CheckCallingStatus(const char* callerName);
static IOSError CallbackAsyncIpc(IOSError, void*);

void __OSInitNet(void) {
    NWC24Err cur_err;
    s32 res;
    OSIOSRev iosRev;

    REXInit();
    __OSGetIOSRev(&iosRev);

    if (iosRev.major <= 4 || iosRev.major == 9) {
        return;
    }

    cur_err = NWC24iPrepareShutdown();

    if (cur_err != NWC24_OK) {
        if (cur_err < NWC24_OK) {
            OSReport("Failed to register network shutdown function. %d\n", cur_err);
        }

        res = NWC24SuspendScheduler();
        if (res < NWC24_OK) {
            OSReport("Failed to suspend the WiiConnect24 scheduler. %d\n", res);
        }
    }

    if (!__OSInIPL) {
        cur_err = NWC24iSynchronizeRtcCounter(0);
        
        if (cur_err != NWC24_OK) {
            OSReport("Failed to synchronize time with network resource managers. %d\n", cur_err);
        }
    }
}

void REXInit(void) {}

NWC24Err NWC24iPrepareShutdown(void) {
    NWC24Err rt = NWC24_OK;
    ShutdownFuncInfo.func = NWC24Shutdown_;
    ShutdownFuncInfo.priority = 110;
    OSRegisterShutdownFunction(&ShutdownFuncInfo);

    if (nwc24ShtFd < 0) {
        rt = NWC24iOpenResourceManager_("NWC24iPrepareShutdown", "/dev/net/kd/request", &nwc24ShtFd, 1);
    }

    nwc24ShtRetryRest = 5;

    if (rt == NWC24_OK) {
        rt = 1;
    }

    return rt;
}

NWC24Err NWC24iSynchronizeRtcCounter(BOOL param_0) {
    NWC24Err rt;
    int rtc;

    rt = GetRTC(&rtc);
    if (rt != NWC24_OK) {
        return rt;
    }

    return NWC24iSetRtcCounter_(rtc, param_0 ? TRUE : FALSE);
}

NWC24Err NWC24SuspendScheduler(void) {
    static u8 susResult[0x20] ATTRIBUTE_ALIGN(32);
    NWC24Err rt = NWC24_OK;
    NWC24Err closeRt = NWC24_OK;
    IOSFd fd;

    rt = CheckCallingStatus("NWC24SuspendScheduler");
    if (rt < NWC24_OK) {
        return rt;
    }

    rt = NWC24iOpenResourceManager_("NWC24SuspendScheduler", "/dev/net/kd/request", &fd, 0);
    if (rt >= NWC24_OK) {
        rt = NWC24iIoctlResourceManager_("NWC24SuspendScheduler", fd, 1, NULL, 0, &susResult, sizeof(susResult));
        if (rt >= NWC24_OK) {
            rt = *(NWC24Err*)susResult;
        }

        closeRt = NWC24iCloseResourceManager_("NWC24SuspendScheduler", fd);
        if (closeRt < NWC24_OK) {
            rt = closeRt;
        }
    }

    return rt;
}

void dummyStrings() {
    OSReport("NWC24ResumeScheduler");
}

NWC24Err NWC24iRequestShutdown(u32 event, s32* result) {
    static u32 shtBuffer[8];
    static u32 shtResult[8];
    NWC24Err ret = NWC24_OK;
    NWC24Err var_r30 = NWC24_OK;

    shtBuffer[0] = event;
    ret = NWC24iIoctlResourceManagerAsync_("NWC24iRequestShutdown", nwc24ShtFd, 0x28, shtBuffer, 0x20, shtResult, 0x20, result);
    return ret;
}

BOOL NWC24Shutdown_(BOOL final, u32 event) {
    static BOOL shuttingdown;
    static s32 result;

    if (final) {
        return TRUE;
    }

    if (shuttingdown) {
        if (NWC24iIsAsyncRequestPending_()) {
            return FALSE;
        }

        if (result >= 0) {
            return TRUE;
        }

        if (nwc24ShtRetryRest > 0) {
            shuttingdown = FALSE;
            nwc24ShtRetryRest--;
        } else {
            OSReport("NWC24Shutdown_: Give up!\n");
            return 1;
        }
    } else {
        if (NWC24iRequestShutdown(event, &result) >= NWC24_OK) {
            shuttingdown = TRUE;
        }
    }

    return FALSE;
}

static u32 nwc24TimeCommonBuffer[8];
static u32 nwc24TimeCommonResult[8];

NWC24Err NWC24iSetRtcCounter_(int rtc, BOOL param_1) {
    NWC24Err rt = NWC24_OK;
    NWC24Err closeRt = NWC24_OK;
    IOSFd fd;

    rt = CheckCallingStatus("NWC24iSetRtcCounter_");
    if (rt < NWC24_OK) {
        return rt;
    }

    rt = NWC24iOpenResourceManager_("NWC24iSetRtcCounter_", "/dev/net/kd/time", &fd, 0);
    if (rt >= NWC24_OK) {
        nwc24TimeCommonBuffer[0] = rtc;
        nwc24TimeCommonBuffer[1] = param_1;

        rt = NWC24iIoctlResourceManager_("NWC24iSetRtcCounter_", fd, 0x17, &nwc24TimeCommonBuffer, sizeof(nwc24TimeCommonBuffer), &nwc24TimeCommonResult, sizeof(nwc24TimeCommonResult));
        if (rt >= NWC24_OK) {
            rt = *(int*)nwc24TimeCommonResult;
        }

        closeRt = NWC24iCloseResourceManager_("NWC24iSetRtcCounter_", fd);
        if (rt >= NWC24_OK) {
            rt = closeRt;
        }
    }

    return rt;
}

NWC24Err NWC24iOpenResourceManager_(const char* callerName, const char* path, IOSFd* fd, u32 flags) {
    if (fd == 0) {
        return -3;
    }

    *fd = IOS_Open(path, flags);
    if (*fd < IOS_ERROR_OK) {
        if (*fd == IOS_ERROR_NOEXISTS) {
            OSReport("%s() %s\n", callerName, "Firmware is still in progress.");
            return -29;
        } else {
            OSReport("%s() %s (%d)\n", callerName, "IOS_Open failure.", *fd);
            return -42;
        }
    }

    return NWC24_OK;
}

NWC24Err NWC24iCloseResourceManager_(const char* callerName, IOSFd fd) {
    int rt = IOS_Close(fd);
    if (rt < IOS_ERROR_OK) {
        OSReport("%s() %s (%d)\n", callerName, "IOS_Close failure.", rt);
        return -42;
    }

    return NWC24_OK;
}

// NOTE: function doesn't exist in TP debug, stub for string data
NWC24Err NWC24iCloseResourceManagerAsync_(const char* callerName, IOSFd fd) {
    OSReport("IOS_CloseAsync failure.");
    return NWC24_OK;
}

NWC24Err NWC24iIoctlResourceManager_(const char* callerName, IOSFd fd, s32 cmd, void* input, u32 inputLen, void* output, u32 outputLen) {
    int rt = IOS_Ioctl(fd, cmd, input, inputLen, output, outputLen);
    if (rt < IOS_ERROR_OK) {
        OSReport("%s() %s (%d)\n", callerName, "IOS_Ioctl failure.", rt);
        return -42;
    }

    return NWC24_OK;
}

NWC24Err NWC24iIoctlResourceManagerAsync_(const char* callerName, IOSFd fd, s32 cmd, void* input, u32 inputLen, void* output, u32 outputLen, void* cbArg) {
    int rt = IOS_IoctlAsync(fd, cmd, input, inputLen, output, outputLen, CallbackAsyncIpc, cbArg);
    if (rt < IOS_ERROR_OK) {
        OSReport("%s() %s (%d)\n", callerName, "IOS_IoctlAsync failure.", rt);
        return -42;
    }

    NWC24iIsRequestPending = TRUE;
    return NWC24_OK;
}

BOOL NWC24iIsAsyncRequestPending_(void) {
    return NWC24iIsRequestPending;
}

static IOSError CallbackAsyncIpc(IOSError param_0, void* param_1) {
    if (param_1 != 0) {
        *(IOSError*)param_1 = param_0;
    }

    NWC24iIsRequestPending = FALSE;
    return 0;
}

static s32 CheckCallingStatus(const char* callerName) {
    if (OSGetCurrentThread() == 0) {
        OSReport("%s() %s\n", callerName, "Illegal status. Unavailable from exception handler.");
        return -1;
    }

    return 0;
}

static NWC24Err GetRTC(int* rtc) {
    OSTime time;
    u32 bias;
    u32 status;
    do {
        status = SCCheckStatus();
        if (status == 2) {
            return -1;
        }
    } while (status != 0);

    bias = SCGetCounterBias();
    time = OSGetTime();
    *rtc = (time / OS_TIMER_CLOCK) - bias;
    return NWC24_OK;
}
