/**
 * m_Do_Reset.cpp
 * Game Reset Management
 */

#include "m_Do/m_Do_Reset.h"
#include "JSystem/JAudio2/JASDvdThread.h"
#include "JSystem/JUtility/JUTGamePad.h"
#include "JSystem/JUtility/JUTXfb.h"
#include "SSystem/SComponent/c_API_controller_pad.h"
#include "dolphin/gx.h"
#include "m_Do/m_Do_audio.h"
#include "m_Do/m_Do_DVDError.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_MemCard.h"

#if !PLATFORM_GCN
#include "revolution/os.h"
#endif

static void my_OSCancelAlarmAll() {}

static void destroyVideo() {
    JUTDestroyVideoManager();
    GXSetDrawDoneCallback(NULL);
    VISetBlack(1);
    VIFlush();
    VIWaitForRetrace();
    return;
}

mDoRstData* mDoRst::mResetData;

void mDoRst_reset(int reset, u32 resetCode, int forceMenu) {
    JUT_ASSERT(83, mDoExt_GetCurrentRunningThread() != 0);
    JUTXfb::getManager()->clearIndex();
    mDoDvdErr_ThdCleanup();
    cAPICPad_recalibrate();

    if (mDoAud_zelAudio_c::isInitFlag()) {
        do {
        } while (!Z2AudioMgr::getInterface()->hasReset());
    }

    if (DVDGetDriveStatus() == DVD_STATE_BUSY) {
        OSAttention("DVD_STATE_BUSY\n");
    }

    JASTaskThread* task_thread = JASDvd::getThreadPointer();
    if (task_thread != NULL) {
        task_thread->pause(true);
        OSThread* thread = task_thread->getThreadRecord();
        if (thread != NULL) {
            OSSuspendThread(thread);
            OSDetachThread(thread);
            OSCancelThread(thread);
        }
    }

    VIWaitForRetrace();
    VIWaitForRetrace();

    OSThread* gxThread = GXGetCurrentGXThread();
    s32 enable = OSDisableInterrupts();

    if (gxThread != OSGetCurrentThread()) {
        OSCancelThread(gxThread);
        GXSetCurrentGXThread();
    }

    GXFlush();
    GXAbortFrame();
    GXDrawDone();
    OSRestoreInterrupts(enable);
    destroyVideo();

    while (!mDoMemCd_isCardCommNone()) {
        VIWaitForRetrace();
    }

    my_OSCancelAlarmAll();
    LCDisable();
#if PLATFORM_GCN
    OSSetSaveRegion(mDoRst::mResetData, (u8*)&mDoRst::getResetData + 0x18);
    OSResetSystem(reset, resetCode, forceMenu);
#else
    if (reset == 2) {
        OSShutdownSystem();
    } else if (reset == 0) {
        OSRestart(resetCode);
    } else {
        OSReturnToMenu();
    }
#endif

    do {
        VIWaitForRetrace();
    } while (true);
}

void checkDiskCallback(s32 result, DVDCommandBlock* block) {
    block->userData = (void*)result;
}

void mDoRst_resetCallBack(int port, void*) {
    if (mDoRst::isReset()) {
        return;
    }
    if (port == -1) {
        cAPICPad_recalibrate();
    } else {
        if (mDoRst::is3ButtonReset()) {
#if PLATFORM_GCN
            JUTGamePad::C3ButtonReset::sResetOccurred = false;
            JUTGamePad::C3ButtonReset::sCallback = mDoRst_resetCallBack;
            JUTGamePad::C3ButtonReset::sCallbackArg = NULL;
#endif
            return;
        }
        mDoRst::on3ButtonReset();
        mDoRst::set3ButtonResetPort(port);
        cAPICPad_recalibrate();
    }

    int check;
#if PLATFORM_GCN
    check = DVDCheckDisk();
#else
    DVDCommandBlock block;
    block.userData = (void*)-1;
    while (DVDCheckDiskAsync(&block, checkDiskCallback));
    do {
        check = (int)block.userData;
    } while (check == -1);
#endif
    if (check == 0) {
        s32 status = DVDGetDriveStatus();
        if (status != DVD_STATE_FATAL_ERROR) {
            mDoRst::onReturnToMenu();
        }
    }
    mDoRst::onReset();
}

void mDoRst_shutdownCallBack() {
    mDoRst::onShutdown();
}
