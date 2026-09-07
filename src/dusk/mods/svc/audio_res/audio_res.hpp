#pragma once
#include "mods/svc/audio_res.h"

namespace dusk::mods {
struct LoadedMod;
}

namespace dusk::mods::svc::audio_res {

namespace wsys {

void frame_end();
void remove_mod(LoadedMod& mod);
void sync_audio_replacements();

extern AudioWaveInfo const default_wave_info;

ModResult insert_replace_wave(
    ModContext* ctx,
    AudioWaveBank bank,
    u16 wave_id,
    char const* file_name,
    AudioWaveInfo const* wave_info,
    AudioWaveHandle* out_handle);

ModResult insert_add_wave(
    ModContext* ctx,
    AudioWaveBank bank,
    char const* file_name,
    AudioWaveInfo const* wave_info,
    AudioWaveHandle* out_handle,
    u16* out_wave_id);

ModResult remove_wave(ModContext* ctx, AudioWaveHandle handle);

}

namespace bst {

void frame_end();
void remove_mod(LoadedMod const& mod);
void sync_audio_replacements();

extern AudioSoundTableEffectInfo const default_effect_info;

ModResult replace_sound_table_effect(
    ModContext* ctx,
    SoundEffectCategory category_id,
    uint16_t effect_id,
    AudioSoundTableEffectInfo const* info,
    AudioSoundTableHandle* out_handle);

ModResult add_sound_table_effect(
    ModContext* ctx,
    SoundEffectCategory category_id,
    AudioSoundTableEffectInfo const* info,
    AudioSoundTableHandle* out_handle,
    uint16_t* out_effect_id);

extern AudioSoundTableStreamInfo const default_stream_info;

ModResult replace_sound_table_stream(
    ModContext* ctx,
    uint16_t stream_id,
    char const* file_path,
    AudioSoundTableStreamInfo const* info,
    AudioSoundTableHandle* out_handle);

ModResult add_sound_table_stream(
    ModContext* ctx,
    char const* file_path,
    AudioSoundTableStreamInfo const* info,
    AudioSoundTableHandle* out_handle,
    uint16_t* out_stream_id);

ModResult remove_sound_table(ModContext* ctx, AudioSoundTableHandle handle);

}

}
