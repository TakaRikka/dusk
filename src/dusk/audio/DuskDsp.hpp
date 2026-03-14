#pragma once

#include "JSystem/JAudio2/JASDSPInterface.h"

#include <array>

#include "SDL3/SDL_audio.h"

namespace dusk::audio {
    constexpr int SampleRate = 32000;

    struct ChannelAuxData {
        s16 hist1;
        s16 hist0;
        SDL_AudioStream* resampleStream;
    };

    extern ChannelAuxData ChannelAux[DSP_CHANNELS];

    using DspSubframe = std::array<s16, DSP_SUBFRAME_SIZE>;

    void DspInit();
    void DspRender(DspSubframe& subframe);
}
