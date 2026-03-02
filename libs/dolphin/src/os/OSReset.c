#include <dolphin/dolphin.h>
#include <dolphin/os.h>

#include "__os.h"

// These macros are copied from OSThread.c. Or ARE they the same
// macros? They dont seem to be in the SDK headers.
#define ENQUEUE_INFO(info, queue)                            \
    do {                                                     \
        OSResetFunctionInfo* __prev = (queue)->tail; \
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
        OSResetFunctionInfo* __next = (info)->next; \
        OSResetFunctionInfo* __prev = (info)->prev; \
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
        OSResetFunctionInfo* __prev;         \
        OSResetFunctionInfo* __next;         \
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

static OSResetFunctionQueue ResetFunctionQueue;
static u32 bootThisDol;

// prototypes
static int CallResetFunctions(int final);
static void Reset(u32 resetCode);

void OSRegisterResetFunction(OSResetFunctionInfo* info) {
    ASSERTLINE(208, info->func);

    ENQUEUE_INFO_PRIO(info, &ResetFunctionQueue);
}

void OSUnregisterResetFunction(OSResetFunctionInfo* info) {
    DEQUEUE_INFO(info, &ResetFunctionQueue);
}

int __OSCallResetFunctions(BOOL final) {
    OSResetFunctionInfo* info;
    int err;
    u32 priority;

    priority = 0;
    err = 0;

    for (info = ResetFunctionQueue.head; info != 0;) {
        if (err != 0 && priority != info->priority)
            break;
        err |= !info->func(final);
        priority = info->priority;
        info = info->next;
    }

    err |= !__OSSyncSram();
    if (err) {
        return 0;
    }
    return 1;
}

#ifdef __GEKKO__
static asm void Reset(u32 resetCode) {
    nofralloc
    b L_000001BC
L_000001A0:
    mfspr r8, HID0
    ori r8, r8, 0x8
    mtspr HID0, r8
    isync
    sync
    nop
    b L_000001C0
L_000001BC:
    b L_000001DC
L_000001C0:
    mftb r5, 268
L_000001C4:
    mftb r6, 268
    subf r7, r5, r6
    cmplwi r7, 0x1124
    blt L_000001C4
    nop
    b L_000001E0
L_000001DC:
    b L_000001FC
L_000001E0:
    lis r8, 0xcc00
    ori r8, r8, 0x3000
    li r4, 0x3
    stw r4, 0x24(r8)
    stw r3, 0x24(r8)
    nop
    b L_00000200
L_000001FC:
    b L_00000208
L_00000200:
    nop
    b L_00000200
L_00000208:
    b L_000001A0
}
#endif

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

void __OSDoHotReset(u32 resetCode) {
    OSDisableInterrupts();
    __VIRegs[1] = 0;
    ICFlashInvalidate();
    Reset(resetCode * 8);
}

void __OSShutdownDevices(BOOL doRecal) {
    int rc;
    BOOL disableRecalibration;

    __OSStopAudioSystem();

    if (!doRecal) {
        disableRecalibration = __PADDisableRecalibration(TRUE);
    }

    do {} while (!__OSCallResetFunctions(FALSE));
    do {} while (!__OSSyncSram());

    OSDisableInterrupts();

    rc = __OSCallResetFunctions(TRUE);
    ASSERTLINE(408, rc);

    LCDisable();
    if (!doRecal) {
        __PADDisableRecalibration(disableRecalibration);
    }

    KillThreads();
}

void OSResetSystem(BOOL reset, u32 resetCode, BOOL forceMenu) {
    OSSram* sram;

    OSDisableScheduler();

    if (reset == TRUE && forceMenu) {
        sram = __OSLockSram();
        sram->flags |= 0x40;
        __OSUnlockSram(1);

        resetCode = 0;
    }

    if (reset == OS_RESET_SHUTDOWN ||
        (reset == OS_RESET_RESTART && (bootThisDol || resetCode + 0x3fff0000 == 0)))
    {
        __OSShutdownDevices(FALSE);
    } else {
        __OSShutdownDevices(TRUE);
    }

    if (reset == OS_RESET_HOTRESET) {
        __OSDoHotReset(resetCode);
    } else if (reset == OS_RESET_RESTART) {
        if (forceMenu == TRUE) {
            OSReport("OSResetSystem(): You can't specify TRUE to forceMenu if you restart. Ignored\n");
        }
        OSEnableScheduler();
        __OSReboot(resetCode, bootThisDol);
    }

    memset(OSPhysicalToCached(0x40), 0, 0xcc - 0x40);
    memset(OSPhysicalToCached(0xd4), 0, 0xe8 - 0xd4);
    memset(OSPhysicalToCached(0xf4), 0, 0xf8 - 0xf4);
    memset(OSPhysicalToCached(0x3000), 0, 0xc0);
    memset(OSPhysicalToCached(0x30c8), 0, 0xd4 - 0xc8);
    memset(OSPhysicalToCached(0x30e2), 0, 1);
}

u32 OSGetResetCode() {
    u32 resetCode;
    if (__OSRebootParams.valid)
        resetCode = 0x80000000 | __OSRebootParams.restartCode;
    else
        resetCode = (__PIRegs[9] & 0xFFFFFFF8) / 8;

    return resetCode;
}

u32 OSSetBootDol(u32 dolOffset) {
    u32 oldDol;

    oldDol = bootThisDol;
    bootThisDol = dolOffset;
    return oldDol;
}
