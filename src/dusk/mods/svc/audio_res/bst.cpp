#include "bst.hpp"

#include "audio_res.hpp"
#include "aurora/lib/logging.hpp"
#include "dusk/mods/loader/loader.hpp"
#include "dusk/mods/svc/id_allocator.hpp"
#include "dusk/mods/svc/internal.hpp"

namespace dusk::mods::svc::audio_res::bst {

namespace {

bool sound_replacements_dirty = false;

aurora::Module Log("dusk::mods::svc::audio_res");

SlotMap<std::shared_ptr<SoundTableReplacementSlot>> sound_replacements;

std::array sound_effect_id_allocator = {
    PlainIdAllocator<u16>(0x1000), // SYSTEM_SE
    PlainIdAllocator<u16>(0x1000), // PLAYER_VOICE
    PlainIdAllocator<u16>(0x1000), // PLAYER_SE
    PlainIdAllocator<u16>(0x1000), // FOOTNOTE_SE
    PlainIdAllocator<u16>(0x1000), // COLLISION_SE
    PlainIdAllocator<u16>(0x1000), // CHARA_VOICE
    PlainIdAllocator<u16>(0x1000), // CHARA_SE
    PlainIdAllocator<u16>(0x1000), // ENEMY_SE
    PlainIdAllocator<u16>(0x1000), // OBJECT_SE
    PlainIdAllocator<u16>(0x1000), // ENV_SE
};

PlainIdAllocator<u16> stream_id_allocator(0x1000);

bool validate_category(SoundEffectCategory const category) {
    return category <= SE_CATEGORY_ENV_SE;
}

uint8_t volume_to_item(float volume) {
    volume = std::clamp(volume, 0.0f, 2.0f);
    return static_cast<uint8_t>(volume * 127);
}

absl::flat_hash_map<SoundEffectKey, std::shared_ptr<SoundEffectReplacementSlot>> active_se_replacements;
absl::flat_hash_map<u16, std::shared_ptr<StreamReplacementSlot>> active_stream_replacements;
std::mutex active_replacements_mutex;

}  // namespace

SoundTableReplacementSlot::SoundTableReplacementSlot(bool const mod_defined, u16 const id)
    : mod_defined(mod_defined), id(id), item() {}

SoundTableReplacementSlot::~SoundTableReplacementSlot() = default;

SoundEffectReplacementSlot::SoundEffectReplacementSlot(
    bool mod_defined, u16 id, SoundEffectCategory category, const AudioSoundTableEffectInfo& info)
    : SoundTableReplacementSlot(mod_defined, id), category(category) {
    item.mPriority = info.priority;
    item.mVolume = volume_to_item(info.volume);
    item.mPitch = info.pitch;

    u32 sw_bit = 0;
    if (info.always_max_priority)
        sw_bit |= SOUND_SW_ALWAYS_MAX_PRIORITY;
    if (info.ignore_distance_volume)
        sw_bit |= SOUND_SW_IGNORE_DISTANCE_VOL;
    if (info.ignore_distance_fx_mix)
        sw_bit |= SOUND_SW_IGNORE_FX_MIX;
    if (info.ignore_pan)
        sw_bit |= SOUND_SW_IGNORE_PAN;
    if (info.ignore_dolby)
        sw_bit |= SOUND_SW_IGNORE_DOLBY;
    sw_bit |= (info.random_volume << SOUND_SW_RANDOM_VOLUME_OFFSET) & SOUND_SW_RANDOM_VOLUME_MASK;
    sw_bit |= (info.random_pitch << SOUND_SW_RANDOM_PITCH_OFFSET) & SOUND_SW_RANDOM_PITCH_MASK;
    sw_bit |= (info.doppler_power << SOUND_SW_DOPPLER_POWER_OFFSET) & SOUND_SW_DOPPLER_POWER_MASK;

    if (info.volume_dist_class < 8) {
        sw_bit |= (info.volume_dist_class << SOUND_SW_VOL_DIST_BIT_OFFSET) & SOUND_SW_VOL_DIST_BIT_MASK;
    } else {
        sw_bit |= ((info.volume_dist_class - 8) << SOUND_SW_VOL_DIST_BIT_2_OFFSET) & SOUND_SW_VOL_DIST_BIT_2_MASK;
    }

    if (info.clamp_min_volume)
        sw_bit |= SOUND_SW_CLAMP_MIN_VOLUME;
    if (info.cull_at_max_distance)
        sw_bit |= SOUND_SW_CULL_AT_MAX_DISTANCE;

    item.mSwBit = sw_bit;
}

u8 SoundEffectReplacementSlot::get_type_id() const {
    return SOUND_TYPEID_SOUND_EFFECT;
}

StreamReplacementSlot::StreamReplacementSlot(
    bool mod_defined, u16 id, char const* file_path, const AudioSoundTableStreamInfo& info)
    : SoundTableReplacementSlot(mod_defined, id), file_path(file_path) {
    stop_on_scene_change = info.stop_on_scene_change;
    item.mPriority = info.priority;
    item.mVolume = volume_to_item(info.volume);

    u16 panParam = 0;
    for (int i = STREAM_MAX_CHILDREN - 1; i >= 0; i--) {
        panParam <<= STRM_CH_SHIFT_;

        switch (info.pan_parameters[i]) {
        case STREAM_PAN_CENTER:
            panParam |= STRM_CH_CENTER;
            break;
        case STREAM_PAN_LEFT:
            panParam |= STRM_CH_LEFT;
            break;
        case STREAM_PAN_RIGHT:
            panParam |= STRM_CH_RIGHT;
            break;
        }
    }
    item.mStreamPanParameters = panParam;
}

u8 StreamReplacementSlot::get_type_id() const {
    return stop_on_scene_change ? SOUND_TYPEID_STREAM_ALT : SOUND_TYPEID_STREAM;
}

std::shared_ptr<SoundTableReplacementSlot> get_override_for(JAISoundID id) {
    switch (id.id_.info.type.parts.sectionID) {
    case 0:
        return get_override_for_se(id);
    case 2:
        return get_override_for_stream(id);
    default:
        return nullptr;
    }
}

std::shared_ptr<SoundEffectReplacementSlot> get_override_for_se(JAISoundID id) {
    if (id.id_.info.type.parts.sectionID != 0) {
        return nullptr;
    }

    std::lock_guard lock(active_replacements_mutex);

    SoundEffectKey const key(
        static_cast<SoundEffectCategory>(id.id_.info.type.parts.groupID),
        id.id_.info.waveID);

    auto const entry = active_se_replacements.find(key);
    if (entry == active_se_replacements.end()) {
        return nullptr;
    }

    return entry->second;
}

std::shared_ptr<StreamReplacementSlot> get_override_for_stream(JAISoundID id) {
    if (id.id_.info.type.parts.sectionID != 2) {
        return nullptr;
    }

    std::lock_guard lock(active_replacements_mutex);

    auto const entry = active_stream_replacements.find(id.id_.info.waveID);
    if (entry == active_stream_replacements.end()) {
        return nullptr;
    }

    return entry->second;
}

void remove_mod(LoadedMod const& mod) {
    sound_replacements.erase_all(mod);

    sound_replacements_dirty = true;
}

void frame_end() {
    if (sound_replacements_dirty) {
        wsys::sync_audio_replacements();
    }
}

void sync_audio_replacements() {
    sound_replacements_dirty = false;

    absl::flat_hash_map<SoundEffectKey, std::shared_ptr<SoundEffectReplacementSlot>> new_se_replacements;
    absl::flat_hash_map<u16, std::shared_ptr<StreamReplacementSlot>> new_stream_replacements;

    for (auto const& mod : ModLoader::instance().active_mods()) {
        sound_replacements.for_each([&](auto, auto const& entry) {
            if (entry.owner != &mod) {
               return;
            }

            std::shared_ptr<SoundTableReplacementSlot> const& value = entry.value;
            auto se = std::dynamic_pointer_cast<SoundEffectReplacementSlot>(value);
            if (se) {
                new_se_replacements.emplace(SoundEffectKey(se->category, se->id), std::move(se));
                return;
            }

            auto stream = std::dynamic_pointer_cast<StreamReplacementSlot>(value);
            if (stream) {
                new_stream_replacements.emplace(stream->id, std::move(stream));
                return;
            }
        });
    }

    std::lock_guard lock(active_replacements_mutex);

    std::exchange(active_se_replacements, std::move(new_se_replacements));
    std::exchange(active_stream_replacements, std::move(new_stream_replacements));
}

static ModResult insert_sound_table_effect_core(ModContext* ctx, SoundEffectCategory category_id,
    uint16_t effect_id, bool mod_defined, AudioSoundTableEffectInfo const* info, AudioSoundTableHandle* out_handle) {
    if (out_handle != nullptr) {
        *out_handle = 0;
    }

    auto mod = mod_from_context(ctx);
    if (mod == nullptr || !validate_category(category_id)) {
        return MOD_INVALID_ARGUMENT;
    }

    if (info == nullptr) {
        info = &default_effect_info;
    }

    auto slot = std::make_shared<SoundEffectReplacementSlot>(mod_defined, effect_id, category_id, *info);
    sound_replacements_dirty = true;

    auto const handle = sound_replacements.emplace(*mod, std::move(slot));
    if (out_handle) {
        *out_handle = handle;
    }

    return MOD_OK;
}

ModResult replace_sound_table_effect(ModContext* ctx, SoundEffectCategory category_id,
    uint16_t effect_id, AudioSoundTableEffectInfo const* info, AudioSoundTableHandle* out_handle) {
    return insert_sound_table_effect_core(ctx, category_id, effect_id, false, info, out_handle);
}

AudioSoundTableEffectInfo const default_effect_info(128, 1, 1);

ModResult add_sound_table_effect(ModContext* ctx, SoundEffectCategory category_id,
    AudioSoundTableEffectInfo const* info, AudioSoundTableHandle* out_handle,
    uint16_t* out_effect_id) {

    if (out_effect_id == nullptr || !validate_category(category_id)) {
        return MOD_INVALID_ARGUMENT;
    }

    assert(category_id < sound_effect_id_allocator.size());

    auto& allocator = sound_effect_id_allocator[category_id];
    auto const effect_id = allocator.alloc();

    auto const result = insert_sound_table_effect_core(ctx, category_id, effect_id, true, info, out_handle);
    if (result != MOD_OK) {
        allocator.free(effect_id);
    }

    *out_effect_id = effect_id;

    return result;
}

static ModResult insert_sound_table_stream_core(
    ModContext* ctx,
    uint16_t stream_id,
    bool mod_defined,
    char const* file_path,
    AudioSoundTableStreamInfo const* info,
    AudioSoundTableHandle* out_handle) {
    if (out_handle != nullptr) {
        *out_handle = 0;
    }

    auto mod = mod_from_context(ctx);
    if (mod == nullptr || file_path == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }

    if (info == nullptr) {
        info = &default_stream_info;
    }

    auto slot = std::make_shared<StreamReplacementSlot>(mod_defined, stream_id, file_path, *info);
    sound_replacements_dirty = true;

    auto const handle = sound_replacements.emplace(*mod, std::move(slot));
    if (out_handle) {
        *out_handle = handle;
    }

    return MOD_OK;
}

AudioSoundTableStreamInfo const default_stream_info(
    128,
    1,
    {
        STREAM_PAN_LEFT,
        STREAM_PAN_RIGHT
    });

ModResult replace_sound_table_stream(ModContext* ctx, uint16_t stream_id,
    char const* file_path,
    AudioSoundTableStreamInfo const* info, AudioSoundTableHandle* out_handle) {
    return insert_sound_table_stream_core(ctx, stream_id, false, file_path, info, out_handle);
}

ModResult add_sound_table_stream(ModContext* ctx,
    char const* file_path, AudioSoundTableStreamInfo const* info,
    AudioSoundTableHandle* out_handle, uint16_t* out_stream_id) {
    if (out_stream_id == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }

    auto const stream_id = stream_id_allocator.alloc();

    auto const result = insert_sound_table_stream_core(ctx, stream_id, true, file_path, info, out_handle);
    if (result != MOD_OK) {
        stream_id_allocator.free(stream_id);
    }

    *out_stream_id = stream_id;

    return result;
}

static PlainIdAllocator<u16>& id_allocator_for(SoundTableReplacementSlot const* slot) {
    if (dynamic_cast<StreamReplacementSlot const*>(slot))
        return stream_id_allocator;

    if (auto const se = dynamic_cast<SoundEffectReplacementSlot const*>(slot))
        return sound_effect_id_allocator[se->id];

    CRASH("Unknown type???");
}

static bool sound_table_remove(LoadedMod const& mod, AudioSoundTableHandle const handle) {
    auto const found = sound_replacements.find_owned(handle, mod);
    if (found == nullptr) {
        return false;
    }

    if (found->value->mod_defined) {
        auto& allocator = id_allocator_for(found->value.get());
        allocator.free(found->value->id);
    }

    sound_replacements.erase_owned(handle, mod);
    return true;
}

ModResult remove_sound_table(ModContext* ctx, AudioSoundTableHandle handle) {
    auto* mod = mod_from_context(ctx);
    if (mod == nullptr || handle == 0) {
        return MOD_INVALID_ARGUMENT;
    }

    if (!sound_table_remove(*mod, handle)) {
        Log.error("[{}] remove sound table failed: unknown handle {}", mod->metadata.id, handle);
        return MOD_INVALID_ARGUMENT;
    }

    return MOD_OK;
}

}  // namespace dusk::mods::svc::audio_res::bst
