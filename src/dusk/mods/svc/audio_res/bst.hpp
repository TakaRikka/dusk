#pragma once

#include <absl/container/flat_hash_map.h>
#include <mutex>

#include "JSystem/JAudio2/JAUSoundTable.h"
#include "mods/svc/audio_res.h"

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

}
