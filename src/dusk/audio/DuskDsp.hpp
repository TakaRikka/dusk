#pragma once

#include "JSystem/JAudio2/JASDSPInterface.h"

#include <array>

using DspSubframe = std::array<s16, DSP_SUBFRAME_SIZE>;

void DuskDspRender(DspSubframe& subframe);
