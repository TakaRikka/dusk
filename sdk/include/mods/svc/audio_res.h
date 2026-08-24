#pragma once

#include <mods/api.h>

#define AUDIO_RES_SERVICE_ID "dev.twilitrealm.dusklight.audio_res"
#define AUDIO_RES_SERVICE_MAJOR 1u
#define AUDIO_RES_SERVICE_MINOR 0u

/*
 * WSYS structs
 */

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
typedef uint64_t AudioSoundTableHandle;

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

/*
 * BST structs
 */

typedef enum SoundEffectCategory : uint8_t {
    SE_CATEGORY_SYSTEM_SE,
    SE_CATEGORY_PLAYER_VOICE,
    SE_CATEGORY_PLAYER_SE,
    SE_CATEGORY_FOOTNOTE_SE,
    SE_CATEGORY_COLLISION_SE,
    SE_CATEGORY_CHARA_VOICE,
    SE_CATEGORY_CHARA_SE,
    SE_CATEGORY_ENEMY_SE,
    SE_CATEGORY_OBJECT_SE,
    SE_CATEGORY_ENV_SE
} SoundEffectCategory;

typedef struct AudioSoundTableEffectInfo {
    uint8_t priority;
    float volume;
    float pitch;

    bool always_max_priority;

    bool ignore_distance_volume;
    bool ignore_distance_fx_mix;
    bool ignore_pan;
    bool ignore_dolby;

    uint8_t random_volume;
    uint8_t random_pitch;

    uint8_t doppler_power;

    uint8_t volume_dist_class;
    bool clamp_min_volume;
    bool cull_at_max_distance;
} AudioSoundTableEffectInfo;

typedef enum StreamPan : uint8_t {
    STREAM_PAN_CENTER,
    STREAM_PAN_LEFT,
    STREAM_PAN_RIGHT,
} StreamPan;

#define STREAM_MAX_CHILDREN 6

typedef struct AudioSoundTableStreamInfo {
    uint8_t priority;
    float volume;
    StreamPan pan_parameters[STREAM_MAX_CHILDREN];
    char const* file_path;
    bool stop_on_scene_change;
} AudioSoundTableStreamInfo;

typedef struct AudioResService {
    ServiceHeader header;

    /*
     * WSYS API
     */

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

    /*
     * BST API
     */

    ModResult (*replace_sound_table_effect)(
        ModContext* ctx,
        SoundEffectCategory category_id,
        uint16_t effect_id,
        AudioSoundTableEffectInfo const* info,
        AudioSoundTableHandle* out_handle);

    ModResult (*add_sound_table_effect)(
        ModContext* ctx,
        SoundEffectCategory category_id,
        AudioSoundTableEffectInfo const* info,
        AudioSoundTableHandle* out_handle,
        uint16_t* out_effect_id);

    ModResult (*replace_sound_table_stream)(
        ModContext* ctx,
        uint16_t stream_id,
        AudioSoundTableStreamInfo const* info,
        AudioSoundTableHandle* out_handle);

    ModResult (*add_sound_table_stream)(
        ModContext* ctx,
        AudioSoundTableStreamInfo const* info,
        AudioSoundTableHandle* out_handle,
        uint16_t* out_stream_id);

    ModResult (*remove_sound_table)(ModContext* ctx, AudioSoundTableHandle handle);
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
