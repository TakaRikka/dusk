#include "Z2AudioCS/SpkSystem.h"

#include "Z2AudioCS/SpkMixingBuffer.h"
#include "Z2AudioCS/SpkSound.h"
#include "Z2AudioCS/SpkSpeakerCtrl.h"
#include "JSystem/JAudio2/JASGadget.h"
#include "JSystem/JAudio2/JASHeapCtrl.h"
#include "JSystem/JKernel/JKRHeap.h"

template<> SpkSystem* JASGlobalInstance<SpkSystem>::sInstance JAS_GLOBAL_INSTANCE_INIT;
template<> SpkSoundHolder* JASGlobalInstance<SpkSoundHolder>::sInstance JAS_GLOBAL_INSTANCE_INIT;

const static s32 cConfigVolumeMax = 15;

SpkSystem::SpkSystem(JKRHeap* heap) : JASGlobalInstance(true) {
    mData = NULL;

    if (!heap) {
        heap = JKRGetCurrentHeap();
    }

    mHeap = heap;
    mMixingBuffer = new (heap, 0) SpkMixingBuffer(heap);
    JUT_ASSERT(35, mMixingBuffer);
    mSoundHolder = new (heap, 0) SpkSoundHolder();
    JUT_ASSERT(38, mSoundHolder);
    SpkSpeakerCtrl::setup();
    SpkSpeakerCtrl::setMixingBuffer(mMixingBuffer);
}

void SpkSystem::setResource(JKRArchive* resArc, u16 param_1, u16 param_2) {
    JUT_ASSERT(71, mHeap);
    JUT_ASSERT(72, resArc);
    mData = new (mHeap, 0) SpkData(resArc);
    JUT_ASSERT(75, mData);
    mData->loadTable(param_1);
    mData->loadWave(param_2);
}

void SpkSystem::framework() {
    JUT_ASSERT(89, mSoundHolder);
#if VERSION != VERSION_WII_USA_R0
    SpkSpeakerCtrl::framework();
#endif
    mSoundHolder->framework();
}

void SpkSystem::startSound(s32 chan, s32 param_1, SpkSoundHandle* handle) {
    JUT_ASSERT(108, chan >= 0);
    JUT_ASSERT(109, chan < 4);
    JUT_ASSERT(110, mSoundHolder);
    mSoundHolder->startSound(chan, param_1, handle);
}

void SpkSystem::startLevelSound(s32 chan, s32 param_1, SpkSoundHandle* handle) {
    JUT_ASSERT(128, chan >= 0);
    JUT_ASSERT(129, chan < 4);
    JUT_ASSERT(130, mSoundHolder);
    JUT_ASSERT(131, handle);
    mSoundHolder->startLevelSound(chan, param_1, handle);
}

void SpkSystem::setMasterVolume(f32 vol) {
    JUT_ASSERT(146, mSoundHolder);
    JUT_ASSERT(147, vol >= 0.f);
    JUT_ASSERT(148, vol <= 1.f);
    mSoundHolder->setMasterVolume(vol);
}

f32 SpkSystem::getMasterVolume(void) {
    return mSoundHolder->getMasterVolume();
}

void SpkSystem::stopAll(s32 chan, s32 msec) {
    JUT_ASSERT(269, mSoundHolder);
    JUT_ASSERT(270, chan < 4);
    JUT_ASSERT(271, msec >= 0);

    if (chan < 0) {
        for (s32 i = 0; i < 4; i++) {
            mSoundHolder->stopAll(i, msec);
        }
    } else {
        mSoundHolder->stopAll(chan, msec);
    }
}

void SpkSystem::setConfigVolume(s32 vol) {
    JUT_ASSERT(307, mSoundHolder);
    JUT_ASSERT(308, vol >= 0);
    JUT_ASSERT(309, vol <= cConfigVolumeMax);

    mSoundHolder->setConfigVolume(vol);
}

void SpkSystem::newSoundMemPool(s32 numOfSound) {
    JUT_ASSERT(351, numOfSound >= 0);
    JASPoolAllocObject<SpkSound>::newMemPool(numOfSound);
}

void SpkSystem::connect(s32 chan) {
    SpkSpeakerCtrl::connect(chan);
}

void SpkSystem::disconnect(s32 chan) {
    SpkSpeakerCtrl::disconnect(chan);
}

void SpkSystem::extensionProcess(s32 chan, s32 param_1) {
    SpkSpeakerCtrl::extensionProcess(chan, param_1);
}
