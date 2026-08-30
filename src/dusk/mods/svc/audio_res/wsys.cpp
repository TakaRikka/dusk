#include "audio_res.hpp"
#include "wsys.hpp"
#include "../slot_map.hpp"
#include "../id_allocator.hpp"
#include "aurora/lib/logging.hpp"
#include "dusk/audio/DuskAudioSystem.h"
#include "dusk/mods/loader/loader.hpp"
#include "helpers/cast.hpp"

namespace dusk::mods::svc::audio_res::wsys {

namespace {

using namespace dusk::helpers::cast;

bool wave_replacements_dirty = false;

aurora::Module Log("dusk::mods::svc::audio_res");

SlotMap<RuntimeWaveReplacementSlot> s_waveReplacements;

constexpr ContainerLoadFunction s_containerLoadFunctions[] = {
    load_wav,
#if DUSK_OPUS
    load_opus,
#endif
};

PlainIdAllocator<u16> sound_effect_id_allocator(5'000);
PlainIdAllocator<u16> music_sample_id_allocator(1'000);

PlainIdAllocator<u16>& id_allocator_for_bank(AudioWaveBank const bank) {
    return bank == AUDIO_WAVE_BANK_SOUND_EFFECTS ? sound_effect_id_allocator : music_sample_id_allocator;
}

bool validate_raw_size(LoadedMod const& mod, std::string const& path, uintptr_t actual_size, AudioRawWave const& raw, u32& sample_count) {
    u32 samples_per_block;
    u32 bytes_per_block;
    switch (raw.format) {
    case AUDIO_WAVE_FORMAT_ADPCM4:
        samples_per_block = 16;
        bytes_per_block = 9;
        break;
    case AUDIO_WAVE_FORMAT_PCM16:
        samples_per_block = 1;
        bytes_per_block = 2;
        break;
    default:
        Log.error("[{}] unimplemented wave format (ADPCM2/PCM8): '{}'", mod.metadata.id, path);
        return false;
    }

    if (actual_size % bytes_per_block != 0) {
        Log.error("[{}] raw file is not divisible by format block size: '{}'", mod.metadata.id, path);
        return false;
    }

    sample_count = (actual_size / bytes_per_block) * samples_per_block;
    return true;
}

ModResult load_raw(
    LoadedMod const& mod, RuntimeWaveReplacementSlot& slot, AudioRawWave const& raw) {
    auto file_contents = mod.bundle->readFile(slot.bundle_path);
    u32 sample_count = 0;

    if (!validate_raw_size(mod, slot.bundle_path, file_contents.size(), raw, sample_count)) {
        Log.error(
            "[{}] replace_wave: file '{}' is a raw .abf file, but raw_wave data was not specified",
            mod.metadata.id, slot.bundle_path);

        return MOD_INVALID_ARGUMENT;
    }

    if (slot.format == AUDIO_WAVE_FORMAT_PCM16) {
        SampleDataPcm16 pcm16;
        pcm16.data.resize(sample_count);
        memcpy(pcm16.data.data(), file_contents.data(), file_contents.size());
        slot.data = std::make_shared<SampleDataPcm16>(std::move(pcm16));
    } else {
        slot.data = std::make_shared<SampleDataU8>(std::move(file_contents));
    }

    slot.sample_count = sample_count;
    slot.sample_rate = raw.sample_rate;
    slot.format = raw.format;
    slot.sample_value_penult = raw.sample_value_penult;
    slot.sample_value_last = raw.sample_value_last;
    return MOD_OK;
}

/**
 * Load an audio file stored in some sort of container format.
 * File type is determined by looking at contents, not extension.
 */
ModResult load_container(LoadedMod const& mod, RuntimeWaveReplacementSlot& slot) {
    auto file_contents = mod.bundle->readFile(slot.bundle_path);

    for (auto const loader : s_containerLoadFunctions) {
        auto const result = loader(mod, slot, file_contents);
        if (result == MOD_OK) {
            return MOD_OK;
        }

        if (result != MOD_UNSUPPORTED) {
            return result;
        }
    }

    return MOD_UNSUPPORTED;
}

JASWaveInfo wave_info_from_slot(RuntimeWaveReplacementSlot const& slot) {
    JASWaveInfo info;
    info.mWaveFormat = static_cast<u8>(slot.format);
    info.mBaseKey = slot.base_key;
    info.mLoopFlag = slot.loop ? 0xFF : 0;
    info.mSampleRate = slot.sample_rate;
    info.mOffsetStart = 0; // Unused but just init it ig.
    info.mOffsetLength = bounded_cast(slot.data->size());
    info.mLoopStartSample = slot.loop_start_sample;
    info.mLoopEndSample = bounded_cast(slot.loop_end_sample);
    info.mSampleCount = bounded_cast(slot.sample_count);
    info.mpLast = slot.sample_value_last;
    info.mpPenult = slot.sample_value_penult;
    // mpLoaded is initialized to point to a 1, so we're good there.
    return info;
}

bool wave_remove(LoadedMod const& mod, AudioWaveHandle const handle) {
    auto const found = s_waveReplacements.find_owned(handle, mod);
    if (found == nullptr) {
        return false;
    }

    if (found->value.mod_defined) {
        auto& allocator = id_allocator_for_bank(found->value.bank);
        allocator.free(found->value.wave_id);
    }

    s_waveReplacements.erase_owned(handle, mod);
    return true;
}

ModResult insert_replace_wave_core(
    ModContext* ctx,
    AudioWaveBank bank,
    u16 wave_id,
    bool mod_defined,
    char const* file_name,
    AudioWaveInfo const* wave_info,
    AudioWaveHandle* out_handle) {
    if (out_handle != nullptr) {
        *out_handle = 0;
    }

    auto mod = mod_from_context(ctx);
    if (mod == nullptr || file_name == nullptr || !is_safe_resource_path(file_name)) {
        return MOD_INVALID_ARGUMENT;
    }

    RuntimeWaveReplacementSlot slot{};
    slot.bundle_path = file_name;
    slot.bank = bank;
    slot.wave_id = wave_id;
    slot.mod_defined = mod_defined;

    ModResult result;
    if (wave_info && wave_info->raw_wave) {
        result = load_raw(*mod, slot, *wave_info->raw_wave);
    } else {
        result = load_container(*mod, slot);
    }
    if (result != MOD_OK) {
        return result;
    }

    if (wave_info != nullptr) {
        slot.base_key = wave_info->base_key;
        slot.loop = wave_info->loop;
        slot.loop_start_sample = wave_info->loop_start_sample;
        slot.loop_end_sample = wave_info->loop_end_sample;

        if (slot.loop_end_sample > slot.sample_count) {
            slot.loop_end_sample = slot.sample_count;
        }

        if (slot.loop_start_sample >= slot.loop_end_sample) {
            Log.error("[{}] wave has start >= end", mod->metadata.id);
            return MOD_INVALID_ARGUMENT;
        }
    } else {
        slot.base_key = AUDIO_RES_DEFAULT_KEY;
        slot.loop = false;
        slot.loop_start_sample = 0;
        slot.loop_end_sample = slot.sample_count;
    }

    wave_replacements_dirty = true;

    const auto handle = s_waveReplacements.emplace(*mod, std::move(slot));
    if (out_handle) {
        *out_handle = handle;
    }

    return MOD_OK;
}


}

absl::flat_hash_map<AudioWaveKey, AudioWaveReplacementValue> s_replacements;
std::mutex s_replacements_mutex;

AudioWaveInfo const default_wave_info(AUDIO_RES_DEFAULT_KEY, false, 0, std::numeric_limits<u32>::max(), nullptr);

ModResult remove_wave(ModContext* ctx, AudioWaveHandle handle) {
    auto* mod = mod_from_context(ctx);
    if (mod == nullptr || handle == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    if (!wave_remove(*mod, handle)) {
        Log.error("[{}] remove wave failed: unknown handle {}", mod->metadata.id, handle);
        return MOD_INVALID_ARGUMENT;
    }
    return MOD_OK;
}

ModResult insert_replace_wave(
    ModContext* ctx,
    AudioWaveBank bank,
    u16 wave_id,
    char const* file_name,
    AudioWaveInfo const* wave_info,
    AudioWaveHandle* out_handle) {

    return insert_replace_wave_core(ctx, bank, wave_id, false, file_name, wave_info, out_handle);
}

ModResult insert_add_wave(
    ModContext* ctx,
    AudioWaveBank bank,
    char const* file_name,
    AudioWaveInfo const* wave_info,
    AudioWaveHandle* out_handle,
    u16* out_wave_id) {

    if (out_wave_id == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }

    *out_wave_id = 0;

    if (bank != AUDIO_WAVE_BANK_SOUND_EFFECTS && bank != AUDIO_WAVE_BANK_MUSIC_SAMPLES) {
        return MOD_INVALID_ARGUMENT;
    }

    auto& allocator = id_allocator_for_bank(bank);
    auto const new_wave_id = allocator.alloc();

    auto const result = insert_replace_wave_core(ctx, bank, new_wave_id, true, file_name, wave_info, out_handle);
    if (result != MOD_OK) {
        allocator.free(new_wave_id);
    }

    *out_wave_id = new_wave_id;
    return result;
}

void remove_mod(LoadedMod& mod) {
    s_waveReplacements.erase_all(mod);

    wave_replacements_dirty = true;
}

void sync_audio_replacements() {
    wave_replacements_dirty = false;

    absl::flat_hash_map<AudioWaveKey, AudioWaveReplacementValue> new_map;

    for (auto const& mod : ModLoader::instance().active_mods()) {
        s_waveReplacements.for_each([&](auto, auto entry) {
             if (entry.owner != &mod) {
                 return;
             }

            auto const wave_info = wave_info_from_slot(entry.value);
            new_map.emplace(
                AudioWaveKey(entry.value.bank, entry.value.wave_id),
                AudioWaveReplacementValue(wave_info, entry.value.data));
        });
    }

    // Log.info("new: {}, old: {}", new_map.size(), s_replacements.size());

    std::lock_guard lock(s_replacements_mutex);
    // Note: new_map will contain the old contents, and is dropped *outside* the lock.
    // As to avoid holding the lock any longer than necessary.
    std::exchange(s_replacements, std::move(new_map));

    // Log.info("new: {}, old: {}", new_map.size(), s_replacements.size());
}

void frame_end() {
    if (wave_replacements_dirty) {
        sync_audio_replacements();
    }
}

AudioWaveReplacementValue::~AudioWaveReplacementValue() = default;
const JASWaveInfo* AudioWaveReplacementValue::getWaveInfo() const {
    return &wave_info;
}

void const* AudioWaveReplacementValue::getAramBaseAddress() const {
    return data->get_data().data();
}

intptr_t AudioWaveReplacementValue::getWavePtr() const {
    return 0;
}

std::unique_ptr<JASSampleDataReference> AudioWaveReplacementValue::getSampleReference() const {
    return std::make_unique<SampleReference>(data);
}

void SampleDataPcm16::be_swap() {
    for (auto& sample : data) {
        ::be_swap(sample);
    }
}


}