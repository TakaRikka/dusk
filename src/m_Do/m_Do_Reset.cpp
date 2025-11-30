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
#include "m_Do/m_Do_MemCard.h"

static void my_OSCancelAlarmAll() {}

static void destroyVideo() {
    JUTVideo::destroyManager();
    GXSetDrawDoneCallback(NULL);
    VISetBlack(1);
    VIFlush();
    VIWaitForRetrace();
    return;
}

mDoRstData* mDoRst::mResetData;

void mDoRst_reset(int param_0, u32 param_1, int param_2) {
    JUTXfb::getManager()->clearIndex();
    mDoDvdErr_ThdCleanup();
    cAPICPad_recalibrate();

    if (mDoAud_zelAudio_c::isInitFlag()) {
        bool audioReset;
        do {
            audioReset = Z2AudioMgr::getInterface()->hasReset();
        } while (!audioReset);
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
    OSSetSaveRegion(mDoRst::mResetData, (u8*)&mDoRst::getResetData + 0x18);
    OSResetSystem(param_0, param_1, param_2);

    do {
        VIWaitForRetrace();
    } while (true);
}

void mDoRst_resetCallBack(int port, void*) {
    if (!mDoRst::isReset()) {
        if (port == -1) {
            cAPICPad_recalibrate();
        } else {
            if (mDoRst::is3ButtonReset()) {
                JUTGamePad::C3ButtonReset::sResetOccurred = false;
                JUTGamePad::C3ButtonReset::sCallback = mDoRst_resetCallBack;
                JUTGamePad::C3ButtonReset::sCallbackArg = NULL;
                return;
            }
            mDoRst::on3ButtonReset();
            mDoRst::set3ButtonResetPort(port);
            cAPICPad_recalibrate();
        }

        if (DVDCheckDisk() == 0 && DVDGetDriveStatus() != DVD_STATE_FATAL_ERROR) {
            mDoRst::onReturnToMenu();
        }
        mDoRst::onReset();
    }
}
