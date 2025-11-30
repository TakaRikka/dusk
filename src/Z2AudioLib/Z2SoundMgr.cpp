#include "Z2AudioLib/Z2SoundMgr.h"
#include "Z2AudioLib/Z2Param.h"
#include "Z2AudioLib/Z2SceneMgr.h"
#include "Z2AudioLib/Z2SpeechMgr2.h"
#include "Z2AudioLib/Z2SeqMgr.h"
#include "Z2AudioLib/Z2SeMgr.h"
#include "Z2AudioLib/Z2SoundInfo.h"
#include "Z2AudioLib/Z2AudioCS.h"
#include "JSystem/JAudio2/JASCalc.h"
#include "JSystem/JAudio2/JASDriverIF.h"
#include "JSystem/JAudio2/JAUSectionHeap.h"
#include "d/d_com_inf_game.h"

u16 seqCallback(JASTrack* track, u16 command) {
    switch (command) {
    case 0x1000:
        Z2GetSeqMgr()->setChildTrackVolume(Z2GetSeqMgr()->getSubBgmHandle(), 3, 1.0f, 0, -1.0f, -1.0f);
        Z2GetSoundMgr()->getSeqMgr()->calc();
        Z2GetSoundMgr()->getSeqMgr()->mixOut();
        break;
    case 0x1001:
        Z2GetSeqMgr()->onFieldBgmJumpStart();
        break;
    case 0x1002:
        Z2GetSeqMgr()->onFieldBgmJumpEnd();
        break;
    case 0x1003:
        Z2GetSpeechMgr2()->speakOneWord(true);
        break;
    case 0x1004:
        Z2GetSeqMgr()->onVariantBgmJumpEnd(true);
        break;
    case 0x1005:
        Z2GetSeqMgr()->onVariantBgmJumpEnd(false);
        break;
    case 0x1006:
        for (JSULink<JAISe>* link = Z2GetSoundMgr()->getSeMgr()->getCategory(5)->getSeList()->getFirst(); link != NULL; link = link->getNext()) {
            JAISe* se = link->getObject();
            switch (se->getID()) {
            case Z2SE_GRO_MV_SINGING:
            case Z2SE_GRR_MV_SINGING:
            case Z2SE_CLERK_MV_SONG_HO:
            case Z2SE_WAD_A_MV_DANCE:
            case Z2SE_MCN_A_V_DANCE:
            case Z2SE_WCN_A_V_DANCE:
                JASTrack* track = se->getTrack();
                JUT_ASSERT(94, track);
                track->writePort(14, 1);
                break;
            }
        }
        break;
    case 0x1015:
        for (JSULink<JAISe>* link = Z2GetSoundMgr()->getSeMgr()->getCategory(5)->getSeList()->getFirst(); link != NULL; link = link->getNext()) {
            JAISe* se = link->getObject();
            switch (se->getID()) {
            case Z2SE_MCN_A_V_DANCE:
            case Z2SE_WCN_A_V_DANCE:
                JASTrack* track = se->getTrack();
                JUT_ASSERT(113, track);
                track->writePort(14, 1);
                break;
            }
        }
        break;
    case 0x1007:
        Z2GetSeqMgr()->changeBgmStatus(7);
        break;
    case 0x1008:
        Z2GetSeqMgr()->changeBgmStatus(8);
        break;
    case 0x1014:
        dComIfGs_staffroll_next_go();
        break;
    }

    if (command >= 0x1009 && command <= 0x1013) {
        JAISoundID sound_id = Z2SE_CSL_TOWN_BAND_VOCAL;
        if (command <= 0x100a) {
            sound_id = Z2SE_CSL_TOWN_BAND_GT_1;
        } else if (command <= 0x100c) {
            sound_id = Z2SE_CSL_TOWN_BAND_GT_2;
        } else if (command <= 0x100f) {
            sound_id = Z2SE_CSL_TOWN_BAND_GT_3;
        } else if (command <= 0x1011) {
            sound_id = Z2SE_CSL_TOWN_BAND_FLUTE;
        }

        for (JSULink<JAISe>* link = Z2GetSoundMgr()->getSeMgr()->getCategory(6)->getSeList()->getFirst(); link != NULL; link = link->getNext()) {
            JAISe* se = link->getObject();
            if (se->getID() == sound_id) {
                JASTrack* track = se->getTrack();
                JUT_ASSERT(159, track);
                track->writePort(14, command);
                break;
            }
        }
    }

    return 0;
}

Z2SoundMgr::Z2SoundMgr() :
    JASGlobalInstance<Z2SoundMgr>(true),
    seMgr_(true),
    seqMgr_(true),
    streamMgr_(true)
{
    soundID_.setAnonymous();
}

bool Z2SoundMgr::startSound(JAISoundID soundID, JAISoundHandle* handle, const JGeometry::TVec3<f32>* posPtr) {
    int soundType = Z2GetSoundInfo()->getSoundType(soundID);
    switch (soundType) {
    case 0: {
        if (Z2GetSoundInfo()->getSwBit(soundID) & 8) {
            OS_REPORT("[Z2SoundMgr::startSound] se seq Mute! id = %08x\n", *(u32*)&soundID);
            Z2GetSeqMgr()->bgmAllMute(3, 0.3f);
            soundID_ = soundID;
        }

        #if PLATFORM_WII || PLATFORM_SHIELD
        if ((u32)soundID == 12) {
            Z2AudioCS::start(0, 0);
        } else if ((u32)soundID == 13) {
            Z2AudioCS::start(0x66, 0);
        }
        #endif
        
        bool result = seMgr_.startSound(soundID, handle, posPtr);
        return result;
    }
    case 1:
        if (soundID == Z2BGM_LUTERA_DEMO) {
            Z2GetSeqMgr()->bgmStart(Z2BGM_LUTERA2, 0, 0);
            Z2GetSeqMgr()->unMuteSceneBgm(0);
            Z2GetSeqMgr()->changeBgmStatus(0);
            return seMgr_.startSound(Z2SE_NO_SOUND, handle, posPtr);
        } else {
            bool loaded = false;
            JAUSectionHeap* sectionHeap = JASGlobalInstance<JAUSectionHeap>::getInstance();
            JUT_ASSERT(228, sectionHeap);

            for (JSULink<JAUSeqDataBlock>* link = sectionHeap->getSectionHeapData().seqDataBlocks.getLoadedBlocks().getFirst(); link != NULL; link = link->getNext()) {
                if (link->getObject()->field_0x10 == soundID) {
                    loaded = true;
                    break;
                }
            }

            if (!loaded) {
                u32 sp2C = sectionHeap->releaseIdleDynamicSeqDataBlock();
                if (!sectionHeap->loadDynamicSeq(soundID, true)) {
                    OS_REPORT("[Z2SoundMgr::startSound] FAILED!!! LOAD SEQ_DATA id: %08x\n", *(u32*)&soundID);

                    for (JSULink<JAUSeqDataBlock>* link = sectionHeap->getSectionHeapData().seqDataBlocks.getFreeBlocks().getFirst(); link != NULL; link = link->getNext()) {
                        OS_REPORT("       freeSeqBlock: %d\n", link->getObject()->field_0x14.size);
                    }
                }

            }

            return seqMgr_.startSound(soundID, handle, posPtr);
        }
    case 2:
        if (soundID == 0x2000001 || soundID == 0x200004d) {
            streamMgr_.stop(180);
            return seMgr_.startSound(Z2SE_NO_SOUND, handle, posPtr);
        }
        if (soundID == 0x2000005) {
            seMgr_.getCategory(9)->pause(false);
            seMgr_.getCategory(9)->getParams()->moveVolume(1.0f, 45);
        }
        return streamMgr_.startSound(soundID, handle, posPtr);
    default:
        char error[32];
        sprintf(error, "Unknown Sound-Type id :%08x\n", *(u32*)&soundID);
        JUT_WARN(277, "%s", error);
    }

    return false;
}

void Z2SoundMgr::calc() {
    seMgr_.calc();
    if (!soundID_.isAnonymous() && !isPlayingSoundID(soundID_)) {
        if (Z2GetSceneMgr()->isSceneExist()) {
            Z2GetSeqMgr()->bgmAllUnMute(3);
        }
        soundID_.setAnonymous();
    }
    seqMgr_.calc();
    streamMgr_.calc();
}

void Z2SoundMgr::setIIR(JAISound* sound, const s16* iir) {
    if (sound != NULL) {
        JASTrack* track = sound->getTrack();
        JUT_ASSERT(340, track);
        for (u32 i = 0; i < 4; i++) {
            JASTrack* child = track->getChild(i);
            if (child != NULL && child->getStatus() == 1) {
                child->setIIR(iir);
                for (u32 j = 0; j < 4; j++) {
                    JASTrack* child2 = child->getChild(j);
                    if (child2 != NULL && child2->getStatus() == 1) {
                        child2->setIIR(iir);
                    }
                }
            }
        }
    }
}

static s16 const cResetFilterTable[8] = {
    0x7FFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

void Z2SoundMgr::setFilterOff(JAISound* sound) {
    if (sound != NULL) {
        JASTrack* track = sound->getTrack();
        JUT_ASSERT(403, track);
        for (u32 i = 0; i < 4; i++) {
            JASTrack* child = track->getChild(i);
            if (child != NULL && child->getStatus() == 1) {
                child->setIIR(cResetFilterTable);
                child->setFIR(cResetFilterTable);
                for (u32 j = 0; j < 4; j++) {
                    JASTrack* child2 = child->getChild(j);
                    if (child2 != NULL && child2->getStatus() == 1) {
                        child2->setIIR(cResetFilterTable);
                        child2->setFIR(cResetFilterTable);
                    }
                }
            }
        }
    }
}

void Z2SoundMgr::resetFilterAll() {
    int i = 1;
    if (Z2Param::SYSTEM_SE_USE_DARK_SE_SETTING) {
        i = 0;
    }

    for (; i < 9; i++) {
        JAISeCategoryMgr* category = seMgr_.getCategory(i);
        int se_count = category->getMaxActiveSe();
        if (se_count > 0) {
            for (JSULink<JAISe>* link = category->getSeList()->getFirst();
                link != NULL && se_count > 0 && !link->getObject()->isFarAway();
                link = link->getNext(), --se_count)
            {
                setFilterOff(link->getObject());
            }
        } else {
            for (JSULink<JAISe>* link = category->getSeList()->getFirst();
                link != NULL && !link->getObject()->isFarAway();
                link = link->getNext())
            {
                setFilterOff(link->getObject());
            }
        }
    }
}

void Z2SoundMgr::mixOut() {
    if (Z2Param::DARK_SE_FILTER_ON && Z2GetSceneMgr()->isInDarkness()) {
        int i = 1;
        if (Z2Param::SYSTEM_SE_USE_DARK_SE_SETTING) {
            i = 0;
        }

        for (; i < 9; i++) {
            JAISeCategoryMgr* category = seMgr_.getCategory(i);
            int se_count = category->getMaxActiveSe();
            if (se_count > 0) {
                for (JSULink<JAISe>* link = category->getSeList()->getFirst();
                    link != NULL && se_count > 0 && !link->getObject()->isFarAway();
                    link = link->getNext(), --se_count)
                {
                    setIIR(link->getObject(), JASCalc::CUTOFF_TO_IIR_TABLE[Z2Param::DARK_SE_LOW_PASS_FILTER_SETTING]);
                }
            } else {
                for (JSULink<JAISe>* link = category->getSeList()->getFirst();
                    link != NULL && !link->getObject()->isFarAway();
                    link = link->getNext())
                {
                    setIIR(link->getObject(), JASCalc::CUTOFF_TO_IIR_TABLE[Z2Param::DARK_SE_LOW_PASS_FILTER_SETTING]);
                }
            }
        }
    }

    seMgr_.mixOut();
    seqMgr_.mixOut();
    streamMgr_.mixOut();
}

void Z2SoundMgr::framework() {
    calc();
    mixOut();
}

void Z2SoundMgr::pauseAllGameSound(bool paused) {
    seMgr_.getCategory(0)->pause(paused);
    seMgr_.getCategory(1)->pause(paused);
    seMgr_.getCategory(2)->pause(paused);
    seMgr_.getCategory(3)->pause(paused);
    seMgr_.getCategory(4)->pause(paused);
    seMgr_.getCategory(5)->pause(paused);
    seMgr_.getCategory(6)->pause(paused);
    seMgr_.getCategory(7)->pause(paused);
    seMgr_.getCategory(8)->pause(paused);
    seMgr_.getCategory(9)->pause(paused);
    seqMgr_.pause(paused);
    streamMgr_.pause(paused);

    if (paused) {
        Z2GetSeMgr()->seMoveVolumeAll(0.0f, 10);
        seqMgr_.getParams()->moveVolume(0.0f, 10);
        streamMgr_.getParams()->moveVolume(0.0f, 10);
    } else {
        Z2GetSeMgr()->seMoveVolumeAll(1.0f, 10);
        seqMgr_.getParams()->moveVolume(1.0f, 10);
        streamMgr_.getParams()->moveVolume(1.0f, 10);
    }
}

void Z2SoundMgr::stopSoundID(JAISoundID soundID) {
    seMgr_.stopSoundID(soundID);
    seqMgr_.stopSoundID(soundID);
    streamMgr_.stopSoundID(soundID);
}

void Z2SoundMgr::stopSync() {
    stop();
    while (seMgr_.getNumActiveSe() > 0 || seqMgr_.getNumActiveSeqs() > 0) {
        calc();
        if (seMgr_.getNumActiveSe() == 0 && seqMgr_.getNumActiveSeqs() == 0) {
            break;
        }
        mixOut();
        JASDriver::waitSubFrame();
    }
}

void Z2SoundMgr::stop() {
    seMgr_.stop();
    seqMgr_.stop();
    streamMgr_.stop();
}

void Z2SoundMgr::initParams() {
    JUT_ASSERT(613, ! seMgr_.isActive());
    JUT_ASSERT(614, ! seqMgr_.isActive());

    seMgr_.initParams();
    seqMgr_.getParams()->init();
    streamMgr_.getParams()->init();
}

void Z2SoundMgr::multiVolumeSoundID(JAISoundID soundID, f32 volume) {
    if (!soundID.isAnonymous() && Z2GetSoundInfo()->getSoundType(soundID) == 0) {
        int category = Z2GetSoundInfo()->getCategory(soundID);
        for (JSULink<JAISe>* link = seMgr_.getCategory(category)->getSeList()->getFirst(); link != NULL; link = link->getNext()) {
            if (link->getObject()->getID() == soundID) {
                link->getObject()->getAuxiliary().params_.mVolume *= volume;
            }
        }
    }
}

bool Z2SoundMgr::isPlayingSoundID(JAISoundID soundID) {
    if (soundID.isAnonymous()) {
        return false;
    }

    if (Z2GetSoundInfo()->getSoundType(soundID) == 0) {
        int category = Z2GetSoundInfo()->getCategory(soundID);
        for (JSULink<JAISe>* link = seMgr_.getCategory(category)->getSeList()->getFirst(); link != NULL; link = link->getNext()) {
            if (link->getObject()->getID() == soundID) {
                return true;
            }
        }
    }

    return false;
}
