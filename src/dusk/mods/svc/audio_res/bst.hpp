#pragma once

#include <absl/container/flat_hash_map.h>
#include <mutex>

#include "JSystem/JAudio2/JAUSoundTable.h"
#include "mods/svc/audio_res.h"

namespace dusk::mods {
struct LoadedMod;
}

namespace dusk::mods::svc::audio_res::bst {

struct SoundTableReplacementSlot {
    bool mod_defined;
    u16 id;
    JAUSoundTableItem item;

    SoundTableReplacementSlot(bool mod_defined, u16 id);

    virtual ~SoundTableReplacementSlot() = 0;

    [[nodiscard]] virtual u8 get_type_id() const = 0;
};

struct SoundEffectReplacementSlot final : SoundTableReplacementSlot {
    SoundEffectCategory category;

    SoundEffectReplacementSlot(bool mod_defined, u16 id, SoundEffectCategory category,
        const AudioSoundTableEffectInfo& info);

    ~SoundEffectReplacementSlot() override = default;

    [[nodiscard]] u8 get_type_id() const override;
};

struct StreamReplacementSlot final : SoundTableReplacementSlot {
    std::string file_path;
    bool stop_on_scene_change;

    StreamReplacementSlot(bool mod_defined, u16 id, char const* file_path, const AudioSoundTableStreamInfo& info);

    ~StreamReplacementSlot() override = default;

    [[nodiscard]] u8 get_type_id() const override;
};

struct SoundEffectKey {
    SoundEffectCategory category;
    u16 id;

    template <typename H>
    friend H AbslHashValue(H h, const SoundEffectKey& k) {
        return H::combine(std::move(h), k.category, k.id);
    }

    [[nodiscard]] bool operator==(const SoundEffectKey& other) const {
        return other.category == category && other.id == id;
    }
};

std::shared_ptr<SoundTableReplacementSlot> get_override_for(JAISoundID id);
std::shared_ptr<SoundEffectReplacementSlot> get_override_for_se(JAISoundID id);
std::shared_ptr<StreamReplacementSlot> get_override_for_stream(JAISoundID id);

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
