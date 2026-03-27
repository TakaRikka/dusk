#pragma once

#include <dolphin/types.h>

namespace dusk::audio {
    /**
     * Initialize the audio system and start playing audio.
     */
    void Initialize();

    void SetMasterVolume(f32 value);

    u32 GetResetCount(int channelIdx);

    f32 VolumeFromU16(u16 value);
}
