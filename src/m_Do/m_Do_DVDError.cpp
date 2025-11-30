/**
 * m_Do_DVDError.cpp
 * DVD Error Handling
 */

#include "m_Do/m_Do_DVDError.h"
#include "JSystem/JKernel/JKRAssertHeap.h"
#include <dolphin/os.h>
#include "m_Do/m_Do_dvd_thread.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_Reset.h"

bool mDoDvdErr_initialized;

static OSThread DvdErr_thread;

#pragma push
#pragma force_active on
static u8 DvdErr_stack[3072] ATTRIBUTE_ALIGN(16);
#pragma pop

static OSAlarm Alarm;

void mDoDvdErr_ThdInit() {
    if (!mDoDvdErr_initialized) {
        OSTime time = OSGetTime();
        OSThread* curThread = OSGetCurrentThread();
        s32 priority = OSGetThreadPriority(curThread);

        OSCreateThread(&DvdErr_thread, (void*(*)(void*))mDoDvdErr_Watch, NULL, DvdErr_stack + sizeof(DvdErr_stack),
                       sizeof(DvdErr_stack), priority - 3, 1);
        OSResumeThread(&DvdErr_thread);
        OSCreateAlarm(&Alarm);
        OSSetPeriodicAlarm(&Alarm, time, OS_BUS_CLOCK / 4, AlarmHandler);

        mDoDvdErr_initialized = true;
    }
}

void mDoDvdErr_ThdCleanup() {
    if (mDoDvdErr_initialized) {
        OSCancelThread(&DvdErr_thread);
        OSCancelAlarm(&Alarm);
        mDoDvdErr_initialized = false;
    }
}

static void mDoDvdErr_Watch(void*) {
    OSDisableInterrupts();
    { JKRThread thread(OSGetCurrentThread(), 0); }

    mDoExt_getAssertHeap()->becomeCurrentHeap();

    do {
        if (DVDGetDriveStatus() == DVD_STATE_FATAL_ERROR) {
            mDoDvdThd::suspend();
        }
        OSSuspendThread(&DvdErr_thread);
    } while (true);
}

static void AlarmHandler(OSAlarm*, OSContext*) {
    OSResumeThread(&DvdErr_thread);
}
