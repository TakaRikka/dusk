#include <revolution.h>
#include <revolution/os.h>
#include <revolution/sc.h>

#include "__os.h"
#include "__dvd.h"

extern BOOL __OSInNandBoot;

// These macros are copied from OSThread.c. Or ARE they the same
// macros? They dont seem to be in the SDK headers.
#define ENQUEUE_INFO(info, queue)                            \
    do {                                                     \
        OSShutdownFunctionInfo* __prev = (queue)->tail; \
        if (__prev == 0) {                                   \
            (queue)->head = (info);                          \
        } else {                                             \
            __prev->next = (info);                           \
        }                                                    \
        (info)->prev = __prev;                               \
        (info)->next = 0;                                    \
        (queue)->tail = (info);                              \
    } while(0);

#define DEQUEUE_INFO(info, queue)                           \
    do {                                                    \
        OSShutdownFunctionInfo* __next = (info)->next; \
        OSShutdownFunctionInfo* __prev = (info)->prev; \
        if (__next == 0) {                                  \
            (queue)->tail = __prev;                         \
        } else {                                            \
            __next->prev = __prev;                          \
        }                                                   \
        if (__prev == 0) {                                  \
            (queue)->head = __next;                         \
        } else {                                            \
            __prev->next = __next;                          \
        }                                                   \
    } while(0);

#define ENQUEUE_INFO_PRIO(info, queue)               \
    do {                                             \
        OSShutdownFunctionInfo* __prev;         \
        OSShutdownFunctionInfo* __next;         \
        for(__next = (queue)->head; __next           \
          && (__next->priority <= (info)->priority); \
                __next = __next->next) ;             \
                                                     \
        if (__next == 0) {                           \
            ENQUEUE_INFO(info, queue);               \
        } else {                                     \
            (info)->next = __next;                   \
            __prev = __next->prev;                   \
            __next->prev = (info);                   \
            (info)->prev = __prev;                   \
            if (__prev == 0) {                       \
                (queue)->head = (info);              \
            } else {                                 \
                __prev->next = (info);               \
            }                                        \
        }                                            \
    } while(0);

static OSShutdownFunctionQueue ShutdownFunctionQueue;
static u32 bootThisDol = 0;
volatile BOOL __OSIsReturnToIdle = FALSE;

// prototypes
static void KillThreads(void);
static int CallResetFunctions(int final);
static void Reset(u32 resetCode);

void OSRegisterShutdownFunction(OSShutdownFunctionInfo* info) {
    ASSERTLINE(373, info->func);

    ENQUEUE_INFO_PRIO(info, &ShutdownFunctionQueue);
}

void OSUnregisterShutdownFunction(OSShutdownFunctionInfo* info) {
    DEQUEUE_INFO(info, &ShutdownFunctionQueue);
}

int __OSCallShutdownFunctions(BOOL final, u32 event) {
    OSShutdownFunctionInfo* info;
    int err;
    u32 priority;

    priority = 0;
    err = 0;

    for (info = ShutdownFunctionQueue.head; info != 0;) {
        if (err != 0 && priority != info->priority)
            break;
        err |= !info->func(final, event);
        priority = info->priority;
        info = info->next;
    }

    err |= !__OSSyncSram();
    return err ? FALSE : TRUE;
}

void __OSShutdownDevices(u32 event) {
    int rc;
    BOOL disableRecalibration;
    BOOL doRecal;

    switch(event) {
    case 0:
    case 5:
    case 6:
        doRecal = FALSE;
        break;
    case 2:
    case 3:
    case 4:
    case 1:
    default:
        doRecal = TRUE;
        break;
    }

    __OSStopAudioSystem();

    if (!doRecal) {
        disableRecalibration = __PADDisableRecalibration(TRUE);
    }

    do {} while (!__OSCallShutdownFunctions(FALSE, event));
    do {} while (!__OSSyncSram());

    OSDisableInterrupts();

    rc = __OSCallShutdownFunctions(TRUE, event);
    ASSERTLINE(491, rc);

    LCDisable();
    if (!doRecal) {
        __PADDisableRecalibration(disableRecalibration);
    }

    KillThreads();
}

u8 __OSGetDiscState(u8 last) {
    u32 flags;

    if (__DVDGetCoverStatus() != 2) {
        return 3;
    } else {
        if ((last == 1) && (__OSGetRTCFlags(&flags) && !flags)) {
            return 1;
        } else {
            return 2;
        }
    }
}

static void KillThreads(void) {
    OSThread* thread;
    OSThread* next;

    for (thread = __OSActiveThreadQueue.head; thread; thread = next) {
        next = thread->linkActive.next;
        switch (thread->state) {
        case 1:
        case 4:
            OSCancelThread(thread);
            continue;
        default:
            continue;
        }
    }
}

void OSShutdownSystem(BOOL reset, u32 resetCode, BOOL forceMenu) {
    SCIdleModeInfo idleModeInfo;
    OSIOSRev iosRev;
    OSStateFlags state;

    memset(&idleModeInfo, 0, sizeof(SCIdleModeInfo));

    SCInit();
    while ( SCCheckStatus() == 1 )
    {}

    SCGetIdleMode(&idleModeInfo);
    __OSStopPlayRecord();
    __OSUnRegisterStateEvent();
    __DVDPrepareReset();
    __OSReadStateFlags(&state);
    state.lastDiscState = __OSGetDiscState(state.lastDiscState);

    if (idleModeInfo.mode == 1) {
        state.lastShutdown  = 5;
    } else {
        state.lastShutdown  = 1;
    }

    __OSClearRTCFlags();
    __OSWriteStateFlags(&state);
    __OSGetIOSRev(&iosRev);

    if (idleModeInfo.mode == 1) {
        __OSIsReturnToIdle = TRUE;

        OSDisableScheduler();
        __OSShutdownDevices(5);
        OSEnableScheduler();
        __OSLaunchMenu();
    } else {
        OSDisableScheduler();
        __OSShutdownDevices(2);
        __OSShutdownToSBY();
    }
}

void OSRestart(u32 resetCode) {
    u8 type = OSGetAppType();
    __OSStopPlayRecord();
    __OSUnRegisterStateEvent();

    if (type == 0x81) {
        OSDisableScheduler();
        __OSShutdownDevices(4);
        OSEnableScheduler();
        __OSRelaunchTitle(resetCode);
    } else if (type == 0x80) {
        OSDisableScheduler();
        __OSShutdownDevices(4);
        OSEnableScheduler();
        __OSReboot(resetCode, bootThisDol);
    }

    OSDisableScheduler();
    __OSShutdownDevices(1);
    __OSHotResetForError();
}

void __OSReturnToMenu(u8 menuMode) {
    OSStateFlags state;
    ESTicketView* var_r30 = NULL;
    int rc;

    __OSStopPlayRecord();
    __OSUnRegisterStateEvent();
    __DVDPrepareReset();
    __OSReadStateFlags(&state);

    state.lastDiscState = __OSGetDiscState(state.lastDiscState);
    state.lastShutdown = 3;
    state.menuMode = menuMode;

    __OSClearRTCFlags();
    __OSWriteStateFlags(&state);

    OSSetArenaLo((void*)0x81280000);
    OSSetArenaHi((void*)0x812F0000);

    rc = ESP_InitLib();
    if (rc) {
        __OSReturnToMenuForError();
    }

    var_r30 = OSAllocFromMEM1ArenaLo(0xE0, 0x20);
    if (var_r30 == 0) {
        __OSReturnToMenuForError();
    }

    memset(var_r30, 0, 0xE0);

    rc = ESP_DiGetTicketView(NULL, var_r30);
    if (!rc && OSPlayTimeIsLimited()) {
        __OSPlayTimeType type = OSPLAYTIME_PERMANENT;
        u32 playTime = -1;
        __OSGetPlayTime(var_r30, &type, &playTime);
        if (playTime == 0) {
            __OSWriteExpiredFlagIfSet();
        }
    }

    OSDisableScheduler();
    __OSShutdownDevices(5);
    OSEnableScheduler();

    __OSLaunchMenu();
    OSDisableScheduler();

    __VISetRGBModeImm();
    __OSHotResetForError();
}

void OSReturnToMenu(void) {
    __OSReturnToMenu(0);
    OSPanic(__FILE__, 895, "OSReturnToMenu(): Falied to boot system menu.\n");
}

void __OSReturnToMenuForError(void) {
    OSStateFlags state;
    __OSReadStateFlags(&state);

    state.lastDiscState = 2;
    state.lastShutdown = 3;

    __OSClearRTCFlags();
    __OSWriteStateFlags(&state);

    __OSLaunchMenu();
    OSDisableScheduler();

    __VISetRGBModeImm();
    __OSHotResetForError();

    OSPanic(__FILE__, 1010, "__OSReturnToMenu(): Falied to boot system menu.\n");
}

void __OSHotResetForError(void) {
    if (__OSInNandBoot || __OSInReboot) {
        __OSInitSTM();
    }

    __OSHotReset();

    OSPanic(__FILE__, 1034, "__OSHotReset(): Falied to reset system.\n");
}

u32 OSGetResetCode() {
    u32 resetCode;
    if (__OSRebootParams.valid)
        resetCode = 0x80000000 | __OSRebootParams.restartCode;
    else
        resetCode = (__PIRegs[9] & 0xFFFFFFF8) / 8;

    return resetCode;
}

void OSResetSystem(int, u32, int) {
    OSPanic(__FILE__, 1185, "OSResetSystem() is obsoleted. It doesn't work any longer.\n");
}
