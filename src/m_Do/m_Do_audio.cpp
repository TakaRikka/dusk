/**
 * m_Do_audio.cpp
 * Audio Management Functions
 */

#include "m_Do/m_Do_audio.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRMemArchive.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "JSystem/JUtility/JUTConsole.h"
#include "Z2AudioLib/Z2AudioCS.h"
#include "d/d_com_inf_game.h"
#include "d/d_debug_viewer.h"
#include "m_Do/m_Do_Reset.h"
#include "m_Do/m_Do_dvd_thread.h"

#if PLATFORM_WII || PLATFORM_SHIELD
#include <revolution/sc.h>
#endif

bool mDoAud_zelAudio_c::mInitFlag;

bool mDoAud_zelAudio_c::mResetFlag;

bool mDoAud_zelAudio_c::mBgmSet;

void mDoAud_zelAudio_c::reset() {
    mBgmSet = false;
}

static mDoAud_zelAudio_c g_mDoAud_zelAudio;

JKRSolidHeap* g_mDoAud_audioHeap;

static mDoDvdThd_toMainRam_c* l_affCommand;

static mDoDvdThd_mountXArchive_c* l_arcCommand;

static mDoDvdThd_mountArchive_c* l_CSarcCommand;

#if DEBUG
static void dummy() {
    OSReport("mDoAud_Create():Initial of Audio system \n");
}
#endif

static void mDoAud_Create() {
    if (l_affCommand == NULL) {
#if DEBUG
        if (!mDoRst::getLogoScnFlag()) {
            return;
        }
#endif
        l_affCommand = mDoDvdThd_toMainRam_c::create("/Audiores/Z2Sound.baa", 2, NULL);

        if (l_affCommand == NULL) {
            return;
        }
#if DEBUG
        JUTReportConsole("mDoAud_Create loading Z2Sound.baa \n");
#endif
    }

    if (l_arcCommand == NULL) {
#if DEBUG
        if (!mDoRst::getLogoScnFlag()) {
            return;
        }
#endif
        l_arcCommand = mDoDvdThd_mountXArchive_c::create("/Audiores/Seqs/Z2SoundSeqs.arc", 0,
                                                         JKRArchive::MOUNT_DVD, NULL);

        if (l_arcCommand == NULL) {
            return;
        }
#if DEBUG
        JUTReportConsole_f("mDoAud_Create loading Z2SoundSeqs.arc\n");
#endif
    }
#if PLATFORM_WII || PLATFORM_SHIELD
    if (l_CSarcCommand == NULL) {
#if DEBUG
        if (!mDoRst::getLogoScnFlag()) {
            return;
        }
#endif

        l_CSarcCommand = mDoDvdThd_mountArchive_c::create("/Audiores/Z2CSRes.arc", 0, JKRHeap::getRootHeap2());

        if (l_CSarcCommand == NULL) {
            return;
        }
#if DEBUG
        JUTReportConsole_f("mDoAud_Create loading Z2CSRes.arc\n");
#endif
    }
#endif
    if (
        l_affCommand->sync()
#if PLATFORM_WII || PLATFORM_SHIELD
        && l_CSarcCommand->sync()
#endif
        && l_arcCommand->sync()
    ) {
#if DEBUG
        JUTReportConsole("mDoAud_Create sync ok.\n");
#endif
        if (g_mDoAud_audioHeap != NULL) {
            s32 groupID = JKRGetCurrentHeap()->changeGroupID(5);
#if PLATFORM_GCN
            const int audioMemSize = 0xA00000;
#else
            const int audioMemSize = 0xB00000;
#endif
            g_mDoAud_zelAudio.init(g_mDoAud_audioHeap, audioMemSize, l_affCommand->getMemAddress(), l_arcCommand->getArchive());
#if PLATFORM_WII || PLATFORM_SHIELD
            Z2AudioCS::init(mDoExt_getGameHeap(), l_CSarcCommand->getArchive(), 15, 1);
#endif
            JKRGetCurrentHeap()->changeGroupID(groupID);
            s32 r28 = g_mDoAud_audioHeap->adjustSize();
        } else {
            // "Cannot initialize audio due to heap allocation failure\n"
            OSReport_Error("ヒープ確保失敗につきオーディオ初期化できません\n");
        }

        (g_mDoAud_zelAudio)
            .setEventBit(dComIfGs_getPEventBit());
#if DEBUG
        JUTReportConsole("mDoAud_Create g_mDoAud_zelAudio.reset before \n");
#endif
        (g_mDoAud_zelAudio).reset();
#if DEBUG
        JUTReportConsole("mDoAud_Create g_mDoAud_zelAudio.reset after\n");
#endif

#if PLATFORM_GCN
        mDoAud_setOutputMode(OSGetSoundMode());
#else
        mDoAud_setOutputMode(SCGetSoundMode());
#endif
        JKRFree(l_affCommand->getMemAddress());
        l_affCommand->destroy();
        l_arcCommand->destroy();
#if PLATFORM_WII || PLATFORM_SHIELD
        l_CSarcCommand->destroy();
        mDoAud_loadStaticWaves();
#endif
        mDoAud_zelAudio_c::onInitFlag();
        mDoDvdThd::SyncWidthSound = true;
#if DEBUG
        JUTReportConsole("mDoAud_Create done!\n");
#endif
    }
}

void mDoAud_Execute() {
    if (!mDoAud_zelAudio_c::isInitFlag()) {
        if (!mDoRst::isShutdown() && !mDoRst::isReturnToMenu()) {
            mDoAud_Create();
        }
    } else {
#if DEBUG
        static const char* l_outputModeName[3] = {"MONO", "STEREO", "DOLBY"};
        static const u32 l_outputMode[3] = {0, 1, 2};
        if (mDoCPd_c::getHoldL(0)) {
            if (mDoCPd_c::getTrigLeft(0)) {
                if (g_mDoAud_zelAudio.getMode() == 0) {
                    g_mDoAud_zelAudio.setMode(2);
                } else {
                    g_mDoAud_zelAudio.setMode(g_mDoAud_zelAudio.getMode() - 1);
                }
            } else if (mDoCPd_c::getTrigRight(0)) {
                if (g_mDoAud_zelAudio.getMode() >= 2) {
                    g_mDoAud_zelAudio.setMode(0);
                } else {
                    g_mDoAud_zelAudio.setMode(g_mDoAud_zelAudio.getMode() + 1);
                }
            }
            g_mDoAud_zelAudio.setOutputMode(l_outputMode[g_mDoAud_zelAudio.getMode()]);
        }
        dDbVw_Report(20, 30, "%s", l_outputModeName[g_mDoAud_zelAudio.getMode()]);
#endif
        g_mDoAud_zelAudio.gframeProcess();
#if DEBUG
        Z2AudioCS::update();
#endif
    }
}

void mDoAud_setSceneName(char const* spot, s32 room, s32 layer) {
    Z2AudioMgr::getInterface()->setSceneName((char*)spot, room, layer);
}

s32 mDoAud_load1stDynamicWave() {
    Z2AudioMgr::getInterface()->load1stDynamicWave();
    return 1;
}

void mDoAud_setFadeOutStart(u8 param_0) {
    Z2AudioMgr::getInterface()->setFadeOutStart((u8)param_0);
}

void mDoAud_setFadeInStart(u8 param_0) {
    Z2AudioMgr::getInterface()->setFadeInStart((u8)param_0);
}

void mDoAud_resetProcess() {
    if (!mDoAud_zelAudio_c::isResetFlag()) {
        Z2AudioMgr::getInterface()->resetProcess(0x1E, false);
        mDoAud_zelAudio_c::onResetFlag();
    }
}

bool mDoAud_resetRecover() {
    if (mDoAud_zelAudio_c::isResetFlag()) {
        if (!Z2AudioMgr::getInterface()->hasReset()) {
            return 0;
        }
        Z2AudioMgr::getInterface()->resetRecover();
        mDoAud_zelAudio_c::offResetFlag();
    }
    return 1;
}
