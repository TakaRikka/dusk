#pragma once

#include <mods/api.h>

#define AUDIO_RES_SERVICE_ID "dev.twilitrealm.dusklight.audio_res"
#define AUDIO_RES_SERVICE_MAJOR 1u
#define AUDIO_RES_SERVICE_MINOR 0u

#define AUDIO_RES_DEFAULT_KEY 0x3C;

typedef enum AudioWaveBank : uint8_t {
    SoundEffects = 0,
    MusicSamples = 1,
} AudioWaveBank;

typedef enum AudioWaveFormat : uint8_t {
    Adpcm4 = 0,
    Adpcm2 = 1,
    Pcm8 = 2,
    Pcm16 = 3,
} AudioWaveFormat;

typedef uint64_t AudioWaveHandle;

/**
 *
 */
typedef struct AudioRawWave {
    AudioWaveFormat format;
    float sample_rate;
    int16_t sample_value_last;
    int16_t sample_value_penult;
} AudioRawWave;

typedef struct AudioWaveInfo {
    uint8_t base_key;
    bool loop;
    uint32_t loop_start_sample;
    uint32_t loop_end_sample;

    AudioRawWave const* raw_wave;
} AudioWaveInfo;

typedef struct AudioResService {
    ServiceHeader header;

    ModResult (*replace_wave)(
        ModContext* ctx,
        AudioWaveBank bank,
        uint16_t wave_id,
        char const* file_name,
        AudioWaveInfo const* wave_info,
        AudioWaveHandle* out_handle);

    ModResult (*add_wave)(
        ModContext* ctx,
        AudioWaveBank bank,
        char const* file_name,
        AudioWaveInfo const* wave_info,
        AudioWaveHandle* out_handle,
        uint16_t* out_wave_id);

    ModResult (*remove_wave)(ModContext* ctx, AudioWaveHandle handle);
} AudioResService;

#ifdef __cplusplus
#include "mods/service.hpp"

template <>
struct mods::ServiceTraits<AudioResService> {
    static constexpr const char* id = AUDIO_RES_SERVICE_ID;
    static constexpr uint16_t major_version = AUDIO_RES_SERVICE_MAJOR;
    static constexpr uint16_t minor_version = AUDIO_RES_SERVICE_MINOR;
};
#endif
