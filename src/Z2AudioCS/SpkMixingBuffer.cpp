#include "Z2AudioCS/SpkMixingBuffer.h"

#include "Z2AudioCS/SpkSound.h"
#include "JSystem/JAudio2/JASCalc.h"
#include "JSystem/JAudio2/JASGadget.h"
#include "JSystem/JAudio2/JASHeapCtrl.h"

SpkMixingBuffer::SpkMixingBuffer(JKRHeap* heap) {
    JUT_ASSERT(25, heap);
    for (s32 chan = 0; chan < ARRAY_SIZE(mBuffer); chan++) {
        mBuffer[chan] = new (heap, 0) s16[cSamplesPerAudioPacket];
        JUT_ASSERT(29, mBuffer[chan]);
        bzeroBuffer(chan);
    }
}

void SpkMixingBuffer::mix(s32 chan, s16* src, s32 len, f32 weight,
                          s32 offset) {
    JUT_ASSERT(61, chan < 4);
    JUT_ASSERT(62, mBuffer[chan]);
    JUT_ASSERT(63, src);
    JUT_ASSERT(64, offset >= 0);
    JUT_ASSERT(65, (offset+len) <= cSamplesPerAudioPacket);

    for (s32 i = 0; i < len; i++) {
        s32 sample = mBuffer[chan][i + offset];
        s16 srcSample = src[i];
        sample += (s32)(srcSample * weight);
        mBuffer[chan][i + offset] = JASCalc::clamp<s16, s32>(sample);
    }
}

s16* SpkMixingBuffer::getSamples(s32 chan) const {
    JUT_ASSERT(97, chan < 4);
    JUT_ASSERT(98, mBuffer[chan]);
    return mBuffer[chan];
}

bool SpkMixingBuffer::update(s32 chan) {
    bzeroBuffer(chan);
    SpkSoundHolder* sound_holder = JASGlobalInstance<SpkSoundHolder>::getInstance();
    JUT_ASSERT(121, sound_holder);
    bool result = sound_holder->update(chan);
    return result;
}

void SpkMixingBuffer::bzeroBuffer(s32 chan) {
    JUT_ASSERT(143, chan < 4);
    JUT_ASSERT(144, mBuffer[chan]);
    JASCalc::bzero(mBuffer[chan], cSamplesPerAudioPacket * sizeof(s16));
}
