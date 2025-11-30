/**
 * m_Do_audio.cpp
 * Audio Management Functions
 */

#include "m_Do/m_Do_audio.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "d/d_com_inf_game.h"
#include "m_Do/m_Do_Reset.h"
#include "m_Do/m_Do_dvd_thread.h"

/* 80450BB8 0001+00 data_80450BB8 None */
/* 80450BB9 0001+00 data_80450BB9 None */
/* 80450BBA 0002+00 data_80450BBA None */
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

static void mDoAud_Create() {
    if (l_affCommand == NULL) {
        l_affCommand = mDoDvdThd_toMainRam_c::create("/Audiores/Z2Sound.baa", 2, NULL);

        if (l_affCommand == NULL) {
            return;
        }
    }

    if (l_arcCommand == NULL) {
        l_arcCommand = mDoDvdThd_mountXArchive_c::create("/Audiores/Seqs/Z2SoundSeqs.arc", 0,
                                                         JKRArchive::MOUNT_DVD, NULL);

        if (l_arcCommand == NULL) {
            return;
        }
    }

    if (l_affCommand->sync() && l_arcCommand->sync()) {
        if (g_mDoAud_audioHeap != NULL) {
            s32 groupID = JKRHeap::sCurrentHeap->changeGroupID(5);
            g_mDoAud_zelAudio
                .init(g_mDoAud_audioHeap, 0xA00000, l_affCommand->getMemAddress(),
                                l_arcCommand->getArchive());
            JKRHeap::sCurrentHeap->changeGroupID(groupID);
            g_mDoAud_audioHeap->adjustSize();
        } else {
            // "Cannot initialize audio due to heap allocation failure\n"
            OSReport_Error("ヒープ確保失敗につきオーディオ初期化できません\n");
        }

        (g_mDoAud_zelAudio)
            .setEventBit(dComIfGs_getPEventBit());
        (g_mDoAud_zelAudio).reset();

        u32 soundMode = OSGetSoundMode();
        Z2AudioMgr::getInterface()->setOutputMode(soundMode);
        JKRHeap::free(l_affCommand->getMemAddress(), NULL);
        l_affCommand->destroy();
        l_arcCommand->destroy();

        mDoAud_zelAudio_c::onInitFlag();
        mDoDvdThd::SyncWidthSound = true;
    }
}

void mDoAud_Execute() {
    if (!mDoAud_zelAudio_c::isInitFlag()) {
        if (!mDoRst::isShutdown() && !mDoRst::isReturnToMenu()) {
            mDoAud_Create();
        }
    } else {
        g_mDoAud_zelAudio.gframeProcess();
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
