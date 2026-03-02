/**
 * m_Do_DVDError.cpp
 * DVD Error Handling
 */

#include "m_Do/m_Do_DVDError.h"
#include "JSystem/JKernel/JKRAssertHeap.h"
#include <os.h>
#include "m_Do/m_Do_dvd_thread.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_Reset.h"

// Added for the sleep workaround
#include <chrono>
#include <thread>

#if PLATFORM_GCN
const int stack_size = 3072;
#else
const int stack_size = 8192;
#endif

bool mDoDvdErr_initialized;

static OSThread DvdErr_thread;

#pragma push
#pragma force_active on
static u8 DvdErr_stack[stack_size] ATTRIBUTE_ALIGN(16);
#pragma pop

// Alarm is not needed for the PC workaround
// static OSAlarm Alarm;

void mDoDvdErr_ThdInit() {
    if (mDoDvdErr_initialized) {
        return;
    }

    // OSTime time = OSGetTime(); // Unused in workaround

    OSCreateThread(&DvdErr_thread, (void* (*)(void*))mDoDvdErr_Watch, NULL,
                   DvdErr_stack + sizeof(DvdErr_stack), sizeof(DvdErr_stack),
                   OSGetThreadPriority(OSGetCurrentThread()) - 3, 1);
    OSResumeThread(&DvdErr_thread);

    // PC Workaround: Disable Alarm logic. The thread will sleep itself.
    // OSCreateAlarm(&Alarm);
    // OSSetPeriodicAlarm(&Alarm, time, OS_BUS_CLOCK / 4, AlarmHandler);

    mDoDvdErr_initialized = true;
}

void mDoDvdErr_ThdCleanup() {
    if (mDoDvdErr_initialized) {
        OSCancelThread(&DvdErr_thread);
        // OSCancelAlarm(&Alarm); // Disable Alarm cancel
        mDoDvdErr_initialized = false;
    }
}

static void mDoDvdErr_Watch(void*) {
#if PLATFORM_GCN
#ifndef TARGET_PC
    OSDisableInterrupts();
#endif
#endif
    JKRThread(OSGetCurrentThread(), 0);

    JKRSetCurrentHeap(mDoExt_getAssertHeap());

    s32 status;
    do {
        status = DVDGetDriveStatus();
        if (status == DVD_STATE_FATAL_ERROR) {
            mDoDvdThd::suspend();
        }

        // PC Workaround:
        // Instead of suspending and waiting for an Alarm (which might not be implemented),
        // we simply sleep for a short duration.
        // OS_BUS_CLOCK / 4 corresponds to roughly 1/4th of a second on GC.
        // We use 250ms here to simulate the periodic check.

        // OSSuspendThread(&DvdErr_thread); // <-- Original causing deadlock without Alarm
        std::this_thread::sleep_for(std::chrono::milliseconds(250));

    } while (true);
}

static void AlarmHandler(OSAlarm*, OSContext*) {
    // This handler is no longer called in the PC workaround
    OSResumeThread(&DvdErr_thread);
}