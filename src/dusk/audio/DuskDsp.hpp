#pragma once

#include "JSystem/JAudio2/JASDSPInterface.h"

#include <array>
#include <cassert>

#include "SDL3/SDL_audio.h"

namespace dusk::audio {
    constexpr int SampleRate = 32000;

    enum class OutputChannel : u8 {
        LEFT,
        RIGHT,
        OutputChannel_MAX
    };

    constexpr

    struct ChannelAuxData {
        s16 hist1;
        s16 hist0;
        SDL_AudioStream* resampleStream;
    };

    extern ChannelAuxData ChannelAux[DSP_CHANNELS];

    using DspSubframe = std::array<f32, DSP_SUBFRAME_SIZE>;

    struct OutputSubframe {
        static constexpr int NUM_CHANNELS = static_cast<int>(OutputChannel::OutputChannel_MAX);

        std::array<DspSubframe, NUM_CHANNELS> channels;

        DspSubframe& operator[](OutputChannel channel) {
            assert(channel < OutputChannel::OutputChannel_MAX);
            return channels[static_cast<int>(channel)];
        }
    };

    void DspInit();
    void DspRender(OutputSubframe& subframe);
}
