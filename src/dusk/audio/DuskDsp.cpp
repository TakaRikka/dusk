#include "DuskDsp.hpp"

#include <limits>

static float SinePos;

void DuskDspRender(DspSubframe& subframe) {
    subframe.fill(0);

    for (auto& elem : subframe) {
        elem = static_cast<s16>(sinf(SinePos) * std::numeric_limits<s16>::max() * 0.2);
        SinePos += 0.05f;
    }

    auto& channels = *reinterpret_cast<std::array<JASDsp::TChannel, DSP_CHANNELS>*>(JASDsp::CH_BUF);
}
