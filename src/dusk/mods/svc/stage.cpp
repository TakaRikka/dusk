#include "stage.hpp"

#include "registry.hpp"

#include "aurora/lib/logging.hpp"
#include "dusk/mods/loader/loader.hpp"
#include "mods/svc/stage.h"

#include "d/d_com_inf_game.h"
#include "dusk/utilities.hpp"

#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace dusk::mods::svc {
namespace {

aurora::Module Log{"dusk::mods::stage"};

constexpr size_t kActrEntrySize = 0x20;
constexpr size_t kTgscEntrySize = 0x23;
constexpr uint8_t kRoomDefault = 0xFF;
constexpr int8_t kLayerDefault = -1;

enum class EditKind : uint8_t { Patch, Delete, Add };

struct ActorEditRecord {
    uint64_t handle = 0;
    LoadedMod* mod = nullptr;
    uint64_t seq = 0;
    uint8_t room = 0;
    int8_t layer = kLayerDefault;
    EditKind kind = EditKind::Patch;
    uint32_t crc = 0;
    std::vector<uint8_t> record;
};

std::unordered_map<std::string, std::vector<ActorEditRecord>> s_edits;
uint64_t s_nextHandle = 1;
uint64_t s_nextSeq = 1;
std::unordered_set<uint32_t> s_warnedRecords;

int32_t compute_mod_priority(const LoadedMod& mod) {
    int32_t index = 0;
    for (const auto& other : ModLoader::instance().mods()) {
        ++index;
        if (&other == &mod) {
            return index;
        }
    }
    return index + 1;
}

const std::vector<ActorEditRecord>* current_stage_edits() {
    if (s_edits.empty()) {
        return nullptr;
    }
    const char* stageName = dComIfGp_getStartStageName();
    if (stageName == nullptr) {
        return nullptr;
    }
    const auto it = s_edits.find(stageName);
    return it != s_edits.end() ? &it->second : nullptr;
}

bool room_matches(const ActorEditRecord& record, uint8_t roomNo) {
    return record.room == roomNo || record.room == kRoomDefault;
}

bool layer_matches(const ActorEditRecord& record, int8_t layer) {
    return record.layer == layer || record.layer == kLayerDefault;
}

}  // namespace

bool stage_apply_actor_edits(void* actorData, void* actorPrm, size_t recordSize, int8_t roomNo) {
    const auto* edits = current_stage_edits();
    if (edits == nullptr) {
        return true;
    }

    uint8_t room = static_cast<uint8_t>(roomNo);
    if (roomNo == -1) {
        room = static_cast<uint8_t>(dComIfGp_getStartStageRoomNo());
    }

    const auto layer = static_cast<int8_t>(dComIfG_play_c::getLayerNo(0));
    const auto crc = utils::crc32(actorData, recordSize);

    const ActorEditRecord* winner = nullptr;
    int32_t winnerPriority = 0;
    const LoadedMod* firstOwner = nullptr;
    for (const auto& record : *edits) {
        if (record.kind == EditKind::Add || !record.mod->active || !layer_matches(record, layer) ||
            !room_matches(record, room))
        {
            continue;
        }

        const bool matches = record.crc == crc && (record.kind == EditKind::Delete ||
                                                      record.record.size() == recordSize);
        if (!matches) {
            continue;
        }

        if (firstOwner == nullptr) {
            firstOwner = record.mod;
        } else if (firstOwner != record.mod && s_warnedRecords.insert(record.crc).second) {
            Log.warn("actor record {:#010x} edited by multiple mods; the later-loaded one wins",
                record.crc);
        }

        const auto priority = compute_mod_priority(*record.mod);
        if (winner == nullptr || priority > winnerPriority ||
            (priority == winnerPriority && record.seq > winner->seq))
        {
            winner = &record;
            winnerPriority = priority;
        }
    }

    if (winner == nullptr) {
        return true;
    }
    if (winner->kind == EditKind::Delete) {
        return false;
    }

    std::memcpy(actorData, winner->record.data(), winner->record.size());
    std::memcpy(actorPrm, winner->record.data() + 8, winner->record.size() - 8);
    return true;
}

void stage_create_new_actors(
    int8_t roomNo, void (*createFn)(void* user, const void* record, size_t size), void* user) {
    const auto* edits = current_stage_edits();
    if (edits == nullptr) {
        return;
    }

    const auto layer = static_cast<int8_t>(dComIfG_play_c::getLayerNo(0));
    for (const auto& record : *edits) {
        if (record.kind == EditKind::Add && record.mod->active &&
            record.room == static_cast<uint8_t>(roomNo) && layer_matches(record, layer))
        {
            createFn(user, record.record.data(), record.record.size());
        }
    }
}

namespace {

ModResult register_edit(
    LoadedMod& mod, const char* stage, ActorEditRecord&& record, uint64_t& outHandle) {
    record.handle = s_nextHandle++;
    record.mod = &mod;
    record.seq = s_nextSeq++;
    outHandle = record.handle;
    s_edits[stage].push_back(std::move(record));
    return MOD_OK;
}

}  // namespace

ModResult stage_patch_actor(LoadedMod& mod, const char* stage, uint8_t room, int8_t layer,
    uint32_t crc, const void* record, size_t recordSize, uint64_t& outHandle) {
    const auto* bytes = static_cast<const uint8_t*>(record);
    return register_edit(mod, stage,
        ActorEditRecord{.room = room,
            .layer = layer,
            .kind = EditKind::Patch,
            .crc = crc,
            .record = {bytes, bytes + recordSize}},
        outHandle);
}

ModResult stage_delete_actor(LoadedMod& mod, const char* stage, uint8_t room, int8_t layer,
    uint32_t crc, uint64_t& outHandle) {
    return register_edit(mod, stage,
        ActorEditRecord{.room = room, .layer = layer, .kind = EditKind::Delete, .crc = crc},
        outHandle);
}

ModResult stage_add_actor(LoadedMod& mod, const char* stage, uint8_t room, int8_t layer,
    const void* record, size_t recordSize, uint64_t& outHandle) {
    const auto* bytes = static_cast<const uint8_t*>(record);
    return register_edit(mod, stage,
        ActorEditRecord{.room = room,
            .layer = layer,
            .kind = EditKind::Add,
            .record = {bytes, bytes + recordSize}},
        outHandle);
}

ModResult stage_remove_actor_edit(LoadedMod& mod, uint64_t handle) {
    for (auto it = s_edits.begin(); it != s_edits.end(); ++it) {
        const auto removed = std::erase_if(it->second,
            [&](const auto& record) { return record.handle == handle && record.mod == &mod; });
        if (removed != 0) {
            if (it->second.empty()) {
                s_edits.erase(it);
            }
            return MOD_OK;
        }
    }
    return MOD_INVALID_ARGUMENT;
}

void stage_remove_mod(LoadedMod& mod) {
    for (auto it = s_edits.begin(); it != s_edits.end();) {
        std::erase_if(it->second, [&](const auto& record) { return record.mod == &mod; });
        it = it->second.empty() ? s_edits.erase(it) : std::next(it);
    }
}

namespace {
constexpr size_t kMaxStageNameLength = 8;

bool is_valid_stage_name(const char* stage) {
    if (stage == nullptr) {
        return false;
    }
    const std::string_view view{stage};
    return !view.empty() && view.size() <= kMaxStageNameLength;
}

bool is_valid_record_size(size_t size) {
    return size == kActrEntrySize || size == kTgscEntrySize;
}

ModResult stage_patch_actor_(ModContext* context, const char* stage, uint8_t room, int8_t layer,
    uint32_t recordCrc, const void* record, size_t recordSize, StageActorHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }

    auto* mod = mod_from_context(context);
    if (mod == nullptr || !is_valid_stage_name(stage) || record == nullptr ||
        !is_valid_record_size(recordSize))
    {
        return MOD_INVALID_ARGUMENT;
    }

    uint64_t handle = 0;
    const auto result =
        stage_patch_actor(*mod, stage, room, layer, recordCrc, record, recordSize, handle);
    if (outHandle != nullptr) {
        *outHandle = handle;
    }

    return result;
}

ModResult stage_delete_actor_(ModContext* context, const char* stage, uint8_t room, int8_t layer,
    uint32_t recordCrc, StageActorHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }

    auto* mod = mod_from_context(context);
    if (mod == nullptr || !is_valid_stage_name(stage)) {
        return MOD_INVALID_ARGUMENT;
    }

    uint64_t handle = 0;
    const auto result = stage_delete_actor(*mod, stage, room, layer, recordCrc, handle);
    if (outHandle != nullptr) {
        *outHandle = handle;
    }
    return result;
}

ModResult stage_add_actor_(ModContext* context, const char* stage, uint8_t room, int8_t layer,
    const void* record, size_t recordSize, StageActorHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }

    auto* mod = mod_from_context(context);
    if (mod == nullptr || !is_valid_stage_name(stage) || room == kRoomDefault ||
        record == nullptr || !is_valid_record_size(recordSize))
    {
        return MOD_INVALID_ARGUMENT;
    }

    uint64_t handle = 0;
    const auto result = stage_add_actor(*mod, stage, room, layer, record, recordSize, handle);
    if (outHandle != nullptr) {
        *outHandle = handle;
    }
    return result;
}

ModResult stage_remove_actor_edit_(ModContext* context, StageActorHandle handle) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || handle == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    return stage_remove_actor_edit(*mod, handle);
}

constexpr StageService s_stageService{
    .header = SERVICE_HEADER(StageService, STAGE_SERVICE_MAJOR, STAGE_SERVICE_MINOR),
    .patch_actor = stage_patch_actor_,
    .delete_actor = stage_delete_actor_,
    .add_actor = stage_add_actor_,
    .remove_actor_edit = stage_remove_actor_edit_,
};

}  // namespace

constinit const ServiceModule g_stageModule{
    .id = STAGE_SERVICE_ID,
    .majorVersion = STAGE_SERVICE_MAJOR,
    .minorVersion = STAGE_SERVICE_MINOR,
    .service = &s_stageService,
    .modDetached = stage_remove_mod,
};

}  // namespace dusk::mods::svc
