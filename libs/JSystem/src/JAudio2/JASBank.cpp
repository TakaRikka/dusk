#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASBank.h"

#include "dusk/mods/svc/audio_res/wsys.hpp"
#include "JSystem/JAudio2/JASAiCtrl.h"
#include "JSystem/JAudio2/JASBasicInst.h"
#include "JSystem/JAudio2/JASBasicWaveBank.h"
#include "JSystem/JAudio2/JASChannel.h"

using namespace dusk::mods::svc::audio_res::wsys;

// NONMATCHING JASPoolAllocObject_MultiThreaded<_> locations
JASChannel* JASBank::noteOn(JASBank const* bank, int program, u8 key, u8 velocity, u16 priority,
                         void (*callback)(u32, JASChannel*, JASDsp::TChannel*, void*),
                         void* callback_data) {
    if (program >= 0xf0) {
        return noteOnOsc(program - 0xf0, key, velocity, priority, callback, callback_data);
    }
    if (!bank) {
        return NULL;
    }
    JASInstParam instParam;
    if (!bank->getInstParam(program, key, velocity, &instParam)) {
        return NULL;
    }
    JASWaveBank* waveBank = bank->getWaveBank();
    if (!waveBank) {
        return NULL;
    }
    JASWaveHandle* waveHandle = waveBank->getWaveHandle(instParam.mWaveId);
    if (!waveHandle) {
        return NULL;
    }
#if TARGET_PC
    auto const wave_key = AudioWaveKey(static_cast<AudioWaveBank>(waveBank->bankId), instParam.mWaveId);
    std::lock_guard lock(s_replacements_mutex);
    auto const found_replacement = s_replacements.find(wave_key);
    if (found_replacement != s_replacements.end()) {
        waveHandle = &found_replacement->second;
    }

    auto const aramBase = waveHandle->getAramBaseAddress();
#endif

    const JASWaveInfo* waveInfo = waveHandle->getWaveInfo();
    if (!waveInfo) {
        return NULL;
    }
    intptr_t wavePtr = waveHandle->getWavePtr();
    if (!wavePtr IF_DUSK(&& !aramBase)) {
        return NULL;
    }

    JASChannel* channel = JKR_NEW JASChannel(callback, callback_data);
    if (!channel) {
        return NULL;
    }
    channel->setPriority(priority);
    channel->mAnon.mWaveInfo = *waveInfo;
    channel->mWaveAramAddress = wavePtr;
#if TARGET_PC
    channel->mAramBaseAddress = aramBase;
    channel->mSampleReference = waveHandle->getSampleReference();
#endif
    channel->mAnon.mChannelType = instParam.mChannelType;
    channel->setBankDisposeID(bank);
    channel->setInitPitch(instParam.mPitch * (waveInfo->mSampleRate / JASDriver::getDacRate()));
    if (instParam.mDontSetKey == 0) {
        channel->setKey(key - waveInfo->mBaseKey);
    }
    channel->setInitVolume(instParam.mVolume);
    channel->setVelocity(velocity);
    channel->setInitPan(instParam.mPan);
    channel->setInitFxmix(instParam.mFxMix);
    channel->setInitDolby(instParam.mDolby);
    for (u32 i = 0; i < instParam.mOscillatorCount; i++) {
        channel->setOscInit(i, instParam.mOscillators[i]);
    }
    channel->setDirectRelease(instParam.mDirectRelease);
    if (!channel->play()) {
        return NULL;
    }
    return channel;
}

// NONMATCHING JASPoolAllocObject_MultiThreaded<_> locations
JASChannel* JASBank::noteOnOsc(int param_0, u8 key, u8 velocity, u16 priority,
                            void (*callback)(u32, JASChannel*, JASDsp::TChannel*, void*),
                            void* callback_data) {
    static JASOscillator::Point const OSC_RELEASE_TABLE[2] = {
        {0x0001, 0x000A, 0x0000},
        {0x000F, 0x0000, 0x0000},
    };
    static const JASOscillator::Data OSC_ENV = {0, 1.0f, NULL, OSC_RELEASE_TABLE, 1.0f, 0.0f};
    JASChannel* channel = JKR_NEW JASChannel(callback, callback_data);
    if (!channel) {
        return NULL;
    }
    channel->setPriority(priority);
    channel->mOscillatorSomething = param_0;
    channel->mAnon.mChannelType = CHANNEL_OSCILLATOR;
    channel->setInitPitch(16736.016f / JASDriver::getDacRate());
    channel->setKey(key - channel->mAnon.mWaveInfo.mBaseKey);
    channel->setVelocity(velocity);
    channel->setOscInit(0, &OSC_ENV);
    if (!channel->play()) {
        return NULL;
    }
    return channel;
}
