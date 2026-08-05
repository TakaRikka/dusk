#pragma once

#include <memory>
#include <mutex>
#include "JSystem/JAudio2/JASWaveInfo.h"
#include "absl/container/flat_hash_map.h"
#include "mods/svc/audio_res.h"

namespace dusk::mods {
struct LoadedMod;
}

namespace dusk::mods::svc::audio_res::wsys {

struct SampleDataBuf {
    virtual ~SampleDataBuf() = default;
    [[nodiscard]] virtual std::span<std::byte const> get_data() const = 0;
    [[nodiscard]] size_t size() const {
        return get_data().size();
    }
};

// Making a separate type for this rather than just using a vector<u8>
// because I don't want aliasing or alignment to eat my face.
struct SampleDataPcm16 : SampleDataBuf {
    std::vector<s16> data;

    [[nodiscard]] std::span<std::byte const> get_data() const override {
        return as_bytes(std::span(data));
    }

    void be_swap();
};

struct SampleDataU8 : SampleDataBuf {
    std::vector<u8> data;

    explicit SampleDataU8(std::vector<u8> data) : data(std::move(data)) {}

    [[nodiscard]] std::span<std::byte const> get_data() const override {
        return as_bytes(std::span(data));
    }
};

struct SampleReference : JASSampleDataReference {
    explicit SampleReference(std::shared_ptr<SampleDataBuf> data) : data(std::move(data)) {}

    std::shared_ptr<SampleDataBuf> data;
};

struct AudioWaveKey {
    AudioWaveBank bank;
    u32 wave_id;

    AudioWaveKey(AudioWaveBank bank, u32 wave_id) : bank(bank), wave_id(wave_id) {};

    template <typename H>
    friend H AbslHashValue(H h, const AudioWaveKey& k) {
        return H::combine(std::move(h), k.bank, k.wave_id);
    }

    [[nodiscard]] bool operator==(const AudioWaveKey& other) const {
       return other.bank == bank && other.wave_id == wave_id;
    }
};

struct AudioWaveReplacementValue : JASWaveHandle {
    AudioWaveReplacementValue(const JASWaveInfo& wave_info, std::shared_ptr<SampleDataBuf> data) : wave_info(wave_info), data(std::move(data)) {};
    ~AudioWaveReplacementValue() override;
    [[nodiscard]] const JASWaveInfo* getWaveInfo() const override;
    [[nodiscard]] intptr_t getWavePtr() const override;
    [[nodiscard]] void const* getAramBaseAddress() const override;
    [[nodiscard]] std::unique_ptr<JASSampleDataReference> getSampleReference() const override;

    JASWaveInfo wave_info;
    std::shared_ptr<SampleDataBuf> data;
};

extern absl::flat_hash_map<AudioWaveKey, AudioWaveReplacementValue> s_replacements;
extern std::mutex s_replacements_mutex;

struct RuntimeWaveReplacementSlot {
    std::string bundle_path;
    AudioWaveBank bank;
    u16 wave_id;

    u8 base_key;
    bool loop;
    u32 loop_start_sample;
    u32 loop_end_sample;

    AudioWaveFormat format;
    f32 sample_rate;
    u32 sample_count;
    s16 sample_value_last;
    s16 sample_value_penult;

    std::shared_ptr<SampleDataBuf> data;
};

using ContainerLoadFunction = ModResult (*)(LoadedMod const& mod, RuntimeWaveReplacementSlot& slot, std::span<u8 const> fileData);

ModResult load_wav(LoadedMod const& mod, RuntimeWaveReplacementSlot& slot, std::span<u8 const> fileData);
ModResult load_opus(LoadedMod const& mod, RuntimeWaveReplacementSlot& slot, std::span<u8 const> fileData);

void frame_end();
void remove_mod(LoadedMod& mod);
void sync_audio_replacements();

ModResult insert_replace_wave(
    ModContext* ctx,
    AudioWaveBank bank,
    u16 wave_id,
    char const* file_name,
    AudioWaveInfo const* wave_info,
    AudioWaveHandle* out_handle);

ModResult remove_wave(ModContext* ctx, AudioWaveHandle handle);

}