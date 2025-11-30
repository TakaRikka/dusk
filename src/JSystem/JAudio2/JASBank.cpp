#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASBank.h"
#include "JSystem/JAudio2/JASAiCtrl.h"
#include "JSystem/JAudio2/JASBasicInst.h"
#include "JSystem/JAudio2/JASBasicWaveBank.h"
#include "JSystem/JAudio2/JASChannel.h"

// NONMATCHING JASPoolAllocObject_MultiThreaded<_> locations
JASChannel* JASBank::noteOn(JASBank const* param_0, int param_1, u8 param_2, u8 param_3, u16 param_4,
                         void (*param_5)(u32, JASChannel*, JASDsp::TChannel*, void*),
                         void* param_6) {
    if (param_1 >= 0xf0) {
        return noteOnOsc(param_1 - 0xf0, param_2, param_3, param_4, param_5, param_6);
    }
    if (!param_0) {
        return NULL;
    }
    JASInstParam stack_60;
    if (!param_0->getInstParam(param_1, param_2, param_3, &stack_60)) {
        return NULL;
    }
    JASWaveBank* waveBank = param_0->getWaveBank();
    if (!waveBank) {
        return NULL;
    }
    JASWaveHandle* waveHandle = waveBank->getWaveHandle(stack_60.field_0x1a);
    if (!waveHandle) {
        return NULL;
    }
    const JASWaveInfo* waveInfo = waveHandle->getWaveInfo();
    if (!waveInfo) {
        return NULL;
    }
    int wavePtr = waveHandle->getWavePtr();
    if (!wavePtr) {
        return NULL;
    }

    JASChannel* channel = new JASChannel(param_5, param_6);
    if (!channel) {
        return NULL;
    }
    channel->setPriority(param_4);
    channel->field_0xdc.field_0x4 = *waveInfo;
    channel->field_0x104 = wavePtr;
    channel->field_0xdc.field_0x0 = stack_60.field_0x1c;
    channel->setBankDisposeID(param_0);
    channel->setInitPitch(stack_60.mPitch * (waveInfo->field_0x04 / JASDriver::getDacRate()));
    if (stack_60.field_0x1e == 0) {
        channel->setKey(param_2 - waveInfo->field_0x01);
    }
    channel->setInitVolume(stack_60.mVolume);
    channel->setVelocity(param_3);
    channel->setInitPan(stack_60.mPan);
    channel->setInitFxmix(stack_60.mFxMix);
    channel->setInitDolby(stack_60.mDolby);
    for (u32 i = 0; i < stack_60.field_0x1d; i++) {
        channel->setOscInit(i, stack_60.field_0x14[i]);
    }
    channel->setDirectRelease(stack_60.field_0x18);
    if (!channel->play()) {
        return NULL;
    }
    return channel;
}

// NONMATCHING JASPoolAllocObject_MultiThreaded<_> locations
JASChannel* JASBank::noteOnOsc(int param_0, u8 param_1, u8 param_2, u16 param_3,
                            void (*param_4)(u32, JASChannel*, JASDsp::TChannel*, void*),
                            void* param_5) {
    static JASOscillator::Point const OSC_RELEASE_TABLE[2] = {
        {0x0001, 0x000A, 0x0000},
        {0x000F, 0x0000, 0x0000},
    };
    static const JASOscillator::Data OSC_ENV = {0, 1.0f, NULL, OSC_RELEASE_TABLE, 1.0f, 0.0f};
    JASChannel* channel = new JASChannel(param_4, param_5);
    if (!channel) {
        return NULL;
    }
    channel->setPriority(param_3);
    channel->field_0x104 = param_0;
    channel->field_0xdc.field_0x0 = 2;
    channel->setInitPitch(16736.016f / JASDriver::getDacRate());
    channel->setKey(param_1 - channel->field_0xdc.field_0x4.field_0x01);
    channel->setVelocity(param_2);
    channel->setOscInit(0, &OSC_ENV);
    if (!channel->play()) {
        return NULL;
    }
    return channel;
}
