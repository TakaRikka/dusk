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

static OSAlarm Alarm;

void mDoDvdErr_ThdInit() {
    if (mDoDvdErr_initialized) {
        return;
    }

    OSTime time = OSGetTime();

    OSCreateThread(&DvdErr_thread, (void*(*)(void*))mDoDvdErr_Watch, NULL, DvdErr_stack + sizeof(DvdErr_stack),
                    sizeof(DvdErr_stack), OSGetThreadPriority(OSGetCurrentThread()) - 3, 1);
    OSResumeThread(&DvdErr_thread);
    OSCreateAlarm(&Alarm);
    OSSetPeriodicAlarm(&Alarm, time, OS_BUS_CLOCK / 4, AlarmHandler);

    mDoDvdErr_initialized = true;
}

void mDoDvdErr_ThdCleanup() {
    if (mDoDvdErr_initialized) {
        OSCancelThread(&DvdErr_thread);
        OSCancelAlarm(&Alarm);
        mDoDvdErr_initialized = false;
    }
}

static void mDoDvdErr_Watch(void*) {
#if PLATFORM_GCN
    OSDisableInterrupts();
#endif
    JKRThread(OSGetCurrentThread(), 0);

    JKRSetCurrentHeap(mDoExt_getAssertHeap());

    s32 status;
    do {
        status = DVDGetDriveStatus();
        if (status == DVD_STATE_FATAL_ERROR) {
            mDoDvdThd::suspend();
        }
        OSSuspendThread(&DvdErr_thread);
    } while (true);
}

static void AlarmHandler(OSAlarm*, OSContext*) {
    OSResumeThread(&DvdErr_thread);
}
