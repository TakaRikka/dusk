#ifndef Z2AUDIOCS_SPKMIXINGBUFFER_H
#define Z2AUDIOCS_SPKMIXINGBUFFER_H

#include "JSystem/JKernel/JKRHeap.h"

static const s32 cSamplesPerAudioPacket = 40;

class SpkMixingBuffer {
public:
    SpkMixingBuffer(JKRHeap* heap);
    void mix(s32 chan, s16* src, s32 len, f32 weight, s32 offset);
    s16* getSamples(s32 chan) const;
    bool update(s32 chan);
    void bzeroBuffer(s32 chan);

private:
    /* 0x00 */ s16* mBuffer[4];
};

#endif /* Z2AUDIOCS_SPKMIXINGBUFFER_H */
