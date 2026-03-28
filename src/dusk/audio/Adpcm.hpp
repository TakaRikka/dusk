#ifndef DUSK_ADPCM_HPP
#define DUSK_ADPCM_HPP

#include <dolphin/types.h>

namespace dusk::audio {
    constexpr u32 Adpcm4FrameSize = 9;
    constexpr u32 AdpcmSampleCount = 16;
    constexpr u32 Adpcm2FrameSize = 5;

    void Adpcm4ToPcm16(const u8* adpcm, size_t adpcmLength, s16* pcm, size_t pcmLength, s16& hist1, s16& hist0);
}

#endif  // DUSK_ADPCM_HPP
