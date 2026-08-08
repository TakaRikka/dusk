#include "dusk/presentation.hpp"

#include "dusk/settings.h"

#include <borealis/presentation.hpp>

#include <algorithm>

namespace dusk::presentation {
namespace {

float preferred_frame_rate() {
    switch (getSettings().game.enableFrameInterpolation.getValue()) {
    case FrameInterpMode::Off:
        return 30.0f;
    case FrameInterpMode::Capped:
        return static_cast<float>(std::max(getSettings().video.maxFrameRate.getValue(), 1));
    case FrameInterpMode::Unlimited:
    default:
        return 0.0f;
    }
}

}  // namespace

void update_frame_rate_preference() {
    borealis::presentation::set_preferred_frame_rate(preferred_frame_rate());
}

}  // namespace dusk::presentation
