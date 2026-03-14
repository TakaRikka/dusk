#pragma once

#include "JSystem/JAudio2/JASDSPInterface.h"

#include <array>

namespace dusk::audio {
    struct ChannelAuxData {
        s16 hist1;
        s16 hist0;
    };

    extern ChannelAuxData ChannelAux[DSP_CHANNELS];

    using DspSubframe = std::array<s16, DSP_SUBFRAME_SIZE>;

    void DspRender(DspSubframe& subframe);
}
