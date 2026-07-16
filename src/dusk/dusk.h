#pragma once

#include <aurora/aurora.h>

#include "aurora/gfx.h"

extern AuroraInfo auroraInfo;

namespace dusk {
    extern AuroraStats lastFrameAuroraStats;
    extern float frameUsagePct;
}

constexpr u32 defaultWindowWidth = 608;
constexpr u32 defaultWindowHeight = 448;

constexpr u32 defaultAspectRatioW = 19;
constexpr u32 defaultAspectRatioH = 14;

static_assert(defaultWindowWidth / defaultAspectRatioW == defaultWindowHeight / defaultAspectRatioH);
