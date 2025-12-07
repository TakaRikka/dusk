/**
 * m_Do_printf.cpp
 * OSReport and printf Helper Functions
 */

#include "m_Do/m_Do_printf.h"
#include "stdio.h"
#include <dolphin/base/PPCArch.h>
#include "m_Do/m_Do_ext.h"

u8 __OSReport_disable;

u8 __OSReport_Error_disable;

u8 __OSReport_Warning_disable;

u8 __OSReport_System_disable;

u8 __OSReport_enable;

#ifdef __GEKKO__
asm void OSSwitchFiberEx(__REGISTER u32 param_0, __REGISTER u32 param_1, __REGISTER u32 param_2, __REGISTER u32 param_3, __REGISTER u32 code, __REGISTER u32 stack) {
    nofralloc
    
    mflr r0
    
    // Back chain
    mr r9, r1
    stwu r9, -8(stack)
    
    // LR save
    mr r1, stack
    stw r0, 4(r9)

    // Call function
    mtlr code
    blrl

    // Switch back
    lwz r5, 0(r1)
    lwz r0, 4(r5)
    mtlr r0
    mr r1, r5

    blr
}
#endif

void my_PutString(const char* string) {
    fputs(string, stdout);
}

void OSVAttention(const char* fmt, va_list args) {
    mDoPrintf_vprintf(fmt, args);
}

void OSAttention(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    mDoPrintf_vprintf(fmt, args);
    va_end(args);
}

void OSReportDisable() {
    __OSReport_disable = true;
}

void OSReportEnable() {
    __OSReport_disable = false;
}

void OSReportForceEnableOn() {
    __OSReport_enable = true;
}

void OSReportForceEnableOff() {
    __OSReport_enable = false;
}

static OSThread* __OSReport_MonopolyThread;

#if DEBUG
OSMutex print_mutex;
u8 print_mutex_initialized;
u8 print_highPriority;
u8 print_threadID;
u8 print_callerPC;
u8 print_callerPCLevel = 3;
#endif

u32 print_counts;

u32 print_errors;

u32 print_warings;

u32 print_systems;

u8 print_initialized;

static bool data_80450BB5;

void OSReportInit() {
    BOOL enabled;

    if (!print_initialized) {
        #if DEBUG
        enabled = OSDisableInterrupts();
        if (!print_mutex_initialized) {
            OSInitMutex(&print_mutex);
            print_mutex_initialized = true;
            my_PutString("\x1b[m\x1b[33m*** OSVReport - OSInitMutex ***\n\x1b[m");
        }
        OSRestoreInterrupts(enabled);
        #endif

        print_initialized = true;
    }
}

static u8 mDoPrintf_FiberStack[2048] ATTRIBUTE_ALIGN(32);

void mDoPrintf_vprintf_Interrupt(char const* fmt, va_list args) {
    BOOL interruptStatus = OSDisableInterrupts();
    if (!data_80450BB5) {
        data_80450BB5 = true;
        uintptr_t var_r29 = (uintptr_t)&mDoPrintf_FiberStack + sizeof(mDoPrintf_FiberStack);
        OSSwitchFiberEx((uintptr_t)fmt, (uintptr_t)args, 0, 0, (uintptr_t)vprintf,
                        var_r29);
        data_80450BB5 = false;
    }
    OSRestoreInterrupts(interruptStatus);
}

void mDoPrintf_vprintf_Thread(char const* fmt, va_list args) {
    #if DEBUG
    OSLockMutex(&print_mutex);
    OSThread* thread = NULL;
    s32 priority;

    if (print_highPriority) {
        thread = OSGetCurrentThread();
        priority = OSGetThreadPriority(thread);
        OSSetThreadPriority(thread, 0);
    }
    #endif

    vprintf(fmt, args);

    #if DEBUG
    if (thread != NULL) {
        OSSetThreadPriority(thread, priority);
    }
    OSUnlockMutex(&print_mutex);
    #endif
}

void mDoPrintf_vprintf(char const* fmt, va_list args) {
    OSThread* currentThread = mDoExt_GetCurrentRunningThread();
    if (currentThread == NULL) {
        mDoPrintf_vprintf_Interrupt(fmt, args);
    } else {
        #if DEBUG
        mDoPrintf_vprintf_Thread(fmt, args);
        #else
        u8* stackPtr = (u8*)OSGetStackPointer();
        if (stackPtr < (u8*)currentThread->stackEnd + 0xA00 || stackPtr > currentThread->stackBase) {
            mDoPrintf_vprintf_Interrupt(fmt, args);
        } else {
            mDoPrintf_vprintf_Thread(fmt, args);
        }
        #endif
    }
}

void mDoPrintf_VReport(const char* fmt, va_list args) {
    if (!print_initialized) {
        OSReportInit();
    }
    if (__OSReport_enable || !__OSReport_disable) {
        OSThread* currentThread = mDoExt_GetCurrentRunningThread();
        if (__OSReport_MonopolyThread == NULL || __OSReport_MonopolyThread == currentThread) {
            mDoPrintf_vprintf(fmt, args);
            print_counts++;
        }
    }
}

void OSVReport(const char* fmt, va_list args) {
    mDoPrintf_VReport(fmt, args);
}

void OSReport(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    OSVReport(fmt, args);
    va_end(args);
}

void OSReport_FatalError(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    OSReportForceEnableOn();
    my_PutString("\x1B[41;37m***** FATAL ERROR *****\n");
    OSVAttention(fmt, args);
    my_PutString("***** FATAL ERROR *****\n\x1B[m");
    OSReportForceEnableOff();
    fflush(stdout);
    print_errors++;

    va_end(args);
}

void OSReport_Error(const char* fmt, ...) {
    print_errors++;
    if (!__OSReport_Error_disable) {
        #if DEBUG
        OSThread* thread = mDoExt_GetCurrentRunningThread();
        if (thread != NULL) {
            OSLockMutex(&print_mutex);
        }
        #endif

        va_list args;
        va_start(args, fmt);
        OSReportForceEnableOn();
        my_PutString("\x1B[41;37m[ERROR]");
        OSVReport(fmt, args);
        my_PutString("\x1B[m");
        OSReportForceEnableOff();
        fflush(stdout);
        va_end(args);

        #if DEBUG
        if (thread != NULL) {
            OSUnlockMutex(&print_mutex);
        }
        #endif
    }
}

void OSReport_Warning(const char* fmt, ...) {
    print_warings++;
    if (!__OSReport_Warning_disable) {
        va_list args;
        va_start(args, fmt);
        OSReportForceEnableOn();
        my_PutString("\x1B[43;30m[WARN]");
        OSVAttention(fmt, args);
        my_PutString("\x1B[m");
        OSReportForceEnableOff();
        fflush(stdout);
        va_end(args);
    }
}

void OSReport_System(const char* fmt, ...) {
    print_systems++;
    if (!__OSReport_System_disable) {
        va_list args;
        va_start(args, fmt);
        OSReportForceEnableOn();
        OSVAttention(fmt, args);
        OSReportForceEnableOff();
        va_end(args);
    }
}

void OSPanic(const char* file, int line, const char* fmt, ...) {
    va_list args;
    u32 i;
    u32* p;
    u32* tmp;

    OSDisableInterrupts();
    va_start(args, fmt);
    mDoPrintf_vprintf(fmt, args);
    va_end(args);
    OSAttention(" in \"%s\" on line %d.\n", file, line);

    OSAttention("\nAddress:      Back Chain    LR Save\n");
    for (i = 0, p = (u32*)OSGetStackPointer(); p && (uintptr_t)p != 0xFFFFFFFF && i++ < 16; p = (u32*)*p) {
        OSAttention("0x%08x:   0x%08x    0x%08x\n", p, p[0], p[1]);
    }

    // force a crash by writing to an invalid address
    tmp = (u32*)0x1234567;
    *tmp = 0x1234567;
    PPCHalt();
}
