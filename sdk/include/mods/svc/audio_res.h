#pragma once

#include <mods/api.h>

#define AUDIO_RES_SERVICE_ID "dev.twilitrealm.dusklight.audio_res"
#define AUDIO_RES_SERVICE_MAJOR 1u
#define AUDIO_RES_SERVICE_MINOR 0u

/*
 * Defines APIs for replacing and adding audio resources.
 *
 * TP's audio engine (JAudioV2) is extremely complex. For more of an overview through its
 * functionality, please see docs/jaudio.md in the repo.
 */

/*
 * WSYS structs
 */

/**
 * "Default" key for wave definitions. Key like musical key, not map keys or whatever.
 */
#define AUDIO_RES_DEFAULT_KEY 0x3C

/**
 * Defines TP's raw wave banks, which can be modified.
 */
typedef enum AudioWaveBank : uint8_t {
    AUDIO_WAVE_BANK_SOUND_EFFECTS = 0,
    AUDIO_WAVE_BANK_MUSIC_SAMPLES = 1,
} AudioWaveBank;

/**
 * Defines formats for raw sample data the DSP can play back.
 */
typedef enum AudioWaveFormat : uint8_t {
    /**
     * 16 samples per 9 bytes custom Nintendo ADPCM.
     */
    AUDIO_WAVE_FORMAT_ADPCM4 = 0,

    /**
     * 16 samples per 5 bytes custom Nintendo ADPCM.
     */
    AUDIO_WAVE_FORMAT_ADPCM2 = 1,

    /**
     * 1-byte-per-sample simple PCM.
     */
    AUDIO_WAVE_FORMAT_PCM8 = 2,

    /**
     * 2-byte-per-sample simple PCM.
     */
    AUDIO_WAVE_FORMAT_PCM16 = 3,
} AudioWaveFormat;

/**
 * Handle to reference wave replacements/additions by mods.
 */
typedef uint64_t AudioWaveHandle;

/**
 * Data needed to load a raw audio data file.
 *
 * @see AudioWaveInfo
 */
typedef struct AudioRawWave {
    /**
     * Format of the raw sample data.
     */
    AudioWaveFormat format;

    /**
     * Sample rate, in Hertz.
     */
    float sample_rate;

    /**
     * Last audio sample before ADPCM loop start point. (unused if not looping)
     */
    int16_t sample_value_last;

    /**
     * Second-to-last audio sample before ADPCM loop start point. (unused if not looping)
     */
    int16_t sample_value_penult;
} AudioRawWave;

/**
 * Metadata needed to play a wave.
 */
typedef struct AudioWaveInfo {
    /**
     * Base key (musical key). Set to AUDIO_RES_DEFAULT_KEY if you don't care.
     */
    uint8_t base_key;

    /**
     * If true, the sample loops.
     */
    bool loop;

    /**
     * Sample number where loop resumes when end is reached.
     * Does nothing if not looping.
     */
    uint32_t loop_start_sample;

    /**
     * End of the wave. Stops playback if not looping.
     * Automatically clamped to the sample count from the file, if greater.
     */
    uint32_t loop_end_sample;

    /**
     * If provided, specifies that the passed file is "raw" and only contains sample data.
     * This must be used if you want to ship ADPCM samples, as other containers do not support that.
     */
    AudioRawWave const* raw_wave;
} AudioWaveInfo;

/*
 * BST structs
 */

/**
 * Game defined categories (groups) for sound effects.
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

/**
 * Data that can be defined for a sound effect in the sound table.
 */
typedef struct AudioSoundTableEffectInfo {
    /**
     * Priority relative to other sound effects. Can affect culling and such if
     * many effects are playing.
     */
    uint8_t priority;

    /**
     * Volume multiplier for this sound effect. Clamped to range 0-2.
     */
    float volume;

    /**
     * Pitch multiplier for this sound effect.
     */
    float pitch;

    /**
     * If true, the effect is always treated as max priority, regardless of factors like distance.
     */
    bool always_max_priority;

    /**
     * Don't calculate volume changes by distance.
     */
    bool ignore_distance_volume;

    /**
     * Don't FX Mix (reverb) changes by distance.
     */
    bool ignore_distance_fx_mix;

    /**
     * Don't calculate panning (left/right balance).
     */
    bool ignore_pan;

    /**
     * Don't calculate dolby (front/back balance).
     */
    bool ignore_dolby;

    /**
     * 0-15 value controlling volume randomization strength.
     */
    uint8_t random_volume;

    /**
     * 0-15 value controlling pitch randomization strength.
     */
    uint8_t random_pitch;

    /**
     * 0-15 value controlling Doppler effect strength.
     */
    uint8_t doppler_power;

    /**
     * Value from 0-15 selecting "volume distance" class. This effectively selects a fixed curve for
     * parameters like volume by distance.
     */
    uint8_t volume_dist_class;

    /**
     * Limit minimum volume of sound (after distance drop-off) to 0.2.
     */
    bool clamp_min_volume;

    /**
     * Treat this sound as "far away" or "culled" at "max distance."
     * Affects things like automatic stopping.
     */
    bool cull_at_max_distance;
} AudioSoundTableEffectInfo;

/**
 * Stereo panning parameters for stream channels.
 */
typedef enum StreamPan : uint8_t {
    STREAM_PAN_CENTER,
    STREAM_PAN_LEFT,
    STREAM_PAN_RIGHT,
} StreamPan;

/**
 * Maximum amount of channels a streamed track can have.
 */
#define STREAM_MAX_CHILDREN 6

/**
 * Data that can be defined for a stream in the sound table.
 */
typedef struct AudioSoundTableStreamInfo {
    /**
     * Unsure if used.
     */
    uint8_t priority;

    /**
     * Volume multiplier for this stream. Clamped to range 0-2.
     */
    float volume;

    /**
     * For each channel in the loaded .ast file, specifies the panning position for said channel.
     */
    StreamPan pan_parameters[STREAM_MAX_CHILDREN];

    /**
     * Whether this stream automatically stops on scene change.
     */
    bool stop_on_scene_change;
} AudioSoundTableStreamInfo;

/**
 * Handle to reference sound table replacements/additions by mods.
 */
typedef uint64_t AudioSoundTableHandle;

/**
 * Defines APIs for replacing and adding audio resources.
 */
typedef struct AudioResService {
    ServiceHeader header;

    /*
     * WSYS API
     */

    /**
     * Default wave info if none is provided: default key, no loop, not raw audio.
     */
    AudioWaveInfo const* default_wave_info;

    /**
     * Replace an existing audio wave in the game. This replacement will follow mod order
     * prioritization.
     *
     * The sound effect will remain permanently resident in memory.
     *
     * Sound effects can be provided as raw samples (if raw_wave data is provided in wave_info),
     * WAVE (.wav) file, or OGG Opus (.opus, if Dusklight is compiled with support). Container
     * formats are detected based on header, not based on file name.
     *
     * @param ctx Pointer to your mod's context.
     * @param bank Which wave bank to replace a sound effect in.
     * @param wave_id ID of the wave to replace. This does *not* directly correlate to sound table
     * entries or JAISound values in any way!
     * @param file_name Path of the audio file to load in the mod's data, e.g. res/foo.opus. This
     * does *not* need to be an overlay file!
     * @param wave_info Optional: metadata for the new wave. If not provided will use reasonable
     * defaults (@ref default_wave_info).
     * @param out_handle Optional: pointer receives the handle for the replacement. This can be used
     * with @ref remove_wave.
     */
    ModResult (*replace_wave)(
        ModContext* ctx,
        AudioWaveBank bank,
        uint16_t wave_id,
        char const* file_name,
        AudioWaveInfo const* wave_info,
        AudioWaveHandle* out_handle);

    /**
     * Add a new audio wave to the game. The service allocates the placed ID for you.
     *
     * The sound effect will remain permanently resident in memory.
     *
     * Sound effects can be provided as raw samples (if raw_wave data is provided in wave_info),
     * WAVE (.wav) file, or OGG Opus (.opus, if Dusklight is compiled with support). Container
     * formats are detected based on header, not based on file name.
     *
     * @param ctx Pointer to your mod's context.
     * @param bank Which wave bank to replace a sound effect in.
     * @param file_name Path of the audio file to load in the mod's data, e.g. res/foo.opus. This does *not* need to be an overlay file!
     * @param wave_info Optional: metadata for the new wave. If not provided will use reasonable defaults (@ref default_wave_info).
     * @param out_handle Optional: pointer receives the handle for the addition. This can be used with @ref remove_wave.
     * @param out_wave_id Receives the allocated ID in the wave bank.
     */
    ModResult (*add_wave)(
        ModContext* ctx,
        AudioWaveBank bank,
        char const* file_name,
        AudioWaveInfo const* wave_info,
        AudioWaveHandle* out_handle,
        uint16_t* out_wave_id);

    /**
     * Remove a wave addition/replacement previously created by this mod.
     *
     * @param ctx Pointer to your mod's context.
     * @param handle The handle identifying which wave to remove.
     */
    ModResult (*remove_wave)(ModContext* ctx, AudioWaveHandle handle);

    /*
     * BST API
     */

    /**
     * Default sound effect info if none is provided: default volume, pitch, medium priority.
     */
    AudioSoundTableEffectInfo const* default_effect_info;

    /**
     * Replace a sound effect's parameters in the sound table. This replacement will follow mod order
     * prioritization.
     *
     * @param ctx Pointer to your mod's context.
     * @param category_id Category of the sound effect.
     * @param effect_id ID of the effect.
     * @param info Optional: new parameters for the sound. Falls back to @ref default_effect_info if not provided.
     * @param out_handle Optional: pointer receives the handle for the replacement. This can be used with @ref remove_sound_table.
     */
    ModResult (*replace_sound_table_effect)(
        ModContext* ctx,
        SoundEffectCategory category_id,
        uint16_t effect_id,
        AudioSoundTableEffectInfo const* info,
        AudioSoundTableHandle* out_handle);

    /**
     * Add a sound effect to the sound table. The service allocates the placed ID for you.
     *
     * @param ctx Pointer to your mod's context.
     * @param category_id Category of the sound effect.
     * @param info Optional: new parameters for the sound. Falls back to @ref default_effect_info if not provided.
     * @param out_handle Optional: pointer receives the handle for the replacement. This can be used with @ref remove_sound_table.
     * @param out_effect_id Receives the allocated ID of the sound effect.
     */
    ModResult (*add_sound_table_effect)(
        ModContext* ctx,
        SoundEffectCategory category_id,
        AudioSoundTableEffectInfo const* info,
        AudioSoundTableHandle* out_handle,
        uint16_t* out_effect_id);

    /**
     * Default sound effect info if none is provided: default volume, medium priority, stereo channels, *no* stop on scene change.
     */
    AudioSoundTableStreamInfo const* default_stream_info;

    /**
     * Replace a sound effect's parameters in the sound table. This replacement will follow mod order
     * prioritization.
     *
     * @param ctx Pointer to your mod's context.
     * @param stream_id ID of the stream.
     * @param file_path Path of the .ast on disc. If you're providing one yourself, use an overlay!
     * @param info Optional: new parameters for the stream. Falls back to @ref default_stream_info if not provided.
     * @param out_handle Optional: pointer receives the handle for the replacement. This can be used with @ref remove_sound_table.
     */
    ModResult (*replace_sound_table_stream)(
        ModContext* ctx,
        uint16_t stream_id,
        char const* file_path,
        AudioSoundTableStreamInfo const* info,
        AudioSoundTableHandle* out_handle);

    /**
     * Add a sound effect to the sound table. The service allocates the placed ID for you.
     *
     * @param ctx Pointer to your mod's context.
     * @param file_path Path of the .ast on disc. If you're providing one yourself, use an overlay!
     * @param info Optional: new parameters for the stream. Falls back to @ref default_stream_info if not provided.
     * @param out_handle Optional: pointer receives the handle for the replacement. This can be used with @ref remove_sound_table.
     * @param out_stream_id Receives the allocated ID of the stream.
     */
    ModResult (*add_sound_table_stream)(
        ModContext* ctx,
        char const* file_path,
        AudioSoundTableStreamInfo const* info,
        AudioSoundTableHandle* out_handle,
        uint16_t* out_stream_id);

    /**
     * Remove a sound table addition/replacement previously created by this mod.
     *
     * @param ctx Pointer to your mod's context.
     * @param handle The handle identifying which wave to remove.
     */
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
