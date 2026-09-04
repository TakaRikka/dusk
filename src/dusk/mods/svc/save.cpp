#include "save.hpp"

#include "item.hpp"
#include "registry.hpp"

#include "aurora/lib/logging.hpp"
#include "d/d_save.h"
#include "dusk/main.h"
#include "dusk/mods/loader/loader.hpp"
#include "dusk/utilities.hpp"
#include "mods/svc/save.h"

#include <fstream>
#include <string_view>

namespace dusk::mods::svc {
namespace {

aurora::Module Log("dusk::mods::save");

constexpr uint32_t kSlotCount = 3;
constexpr size_t kQuestLogSize = 0xA94;
static_assert(kQuestLogSize == QUEST_LOG_SIZE);
constexpr int kSidecarVersion = 1;
constexpr const char* kSidecarName = "mod_saves.json";
constexpr size_t kMaxBlobNameLength = 256;

using BlobMap = std::map<std::string, std::vector<uint8_t>>;

struct SlotStore {
    bool snapshotValid = false;
    uint32_t snapshotCrc = 0;
    std::map<std::string, BlobMap> mods;
};

struct SaveObserverRecord {
    uint64_t handle = 0;
    LoadedMod* mod = nullptr;
    SaveEventFn onNewSave = nullptr;
    SaveEventFn onLoaded = nullptr;
    SaveEventFn onWritten = nullptr;
    void* userData = nullptr;
};

std::array<SlotStore, kSlotCount> s_slots;
int32_t s_currentSlot = -1;
bool s_sidecarLoaded = false;
std::vector<SaveObserverRecord> s_observers;
uint64_t s_nextHandle = 1;

std::filesystem::path sidecar_path() {
    return dusk::ConfigPath / kSidecarName;
}

void load_sidecar() {
    if (s_sidecarLoaded) {
        return;
    }
    s_sidecarLoaded = true;
    std::ifstream in{sidecar_path()};
    if (!in.is_open()) {
        return;
    }
    try {
        const auto json = nlohmann::json::parse(in);
        if (json.value("version", 0) != kSidecarVersion) {
            Log.warn(
                "mod save sidecar has unknown version {}; ignoring it", json.value("version", 0));
            return;
        }
        const auto& slots = json.at("slots");
        for (uint32_t slot = 0; slot < kSlotCount && slot < slots.size(); ++slot) {
            auto& store = s_slots[slot];
            const auto& slotJson = slots[slot];
            if (slotJson.contains("snapshot_crc32")) {
                store.snapshotValid = true;
                store.snapshotCrc = slotJson["snapshot_crc32"].get<uint32_t>();
            }
            const auto modsJson = slotJson.value("mods", nlohmann::json::object());
            for (const auto& [modId, blobs] : modsJson.items()) {
                for (const auto& [name, encoded] : blobs.items()) {
                    std::vector<uint8_t> bytes;
                    if (!utils::base64_decode(encoded.get<std::string>(), bytes)) {
                        Log.warn("mod save sidecar: bad blob '{}/{}' in slot {}; dropped", modId,
                            name, slot);
                        continue;
                    }
                    s_slots[slot].mods[modId][name] = std::move(bytes);
                }
            }
        }
    } catch (const std::exception& e) {
        Log.error("failed to read mod save sidecar: {}", e.what());
    }
}

void flush_sidecar() {
    nlohmann::json slots = nlohmann::json::array();
    for (const auto& store : s_slots) {
        nlohmann::json slotJson = nlohmann::json::object();
        if (store.snapshotValid) {
            slotJson["snapshot_crc32"] = store.snapshotCrc;
        }
        nlohmann::json mods = nlohmann::json::object();
        for (const auto& [modId, blobs] : store.mods) {
            if (blobs.empty()) {
                continue;
            }
            nlohmann::json blobsJson = nlohmann::json::object();
            for (const auto& [name, bytes] : blobs) {
                blobsJson[name] = utils::base64_encode(bytes);
            }
            mods[modId] = std::move(blobsJson);
        }
        slotJson["mods"] = std::move(mods);
        slots.push_back(std::move(slotJson));
    }
    const nlohmann::json json{{"version", kSidecarVersion}, {"slots", std::move(slots)}};

    const auto path = sidecar_path();
    const auto tempPath = path.string() + ".tmp";
    try {
        {
            std::ofstream out{tempPath, std::ios::trunc};
            out << json.dump(2);
            if (!out.good()) {
                throw std::runtime_error("write failed");
            }
        }
        std::filesystem::rename(tempPath, path);
    } catch (const std::exception& e) {
        Log.error("failed to write mod save sidecar: {}", e.what());
        std::error_code ec;
        std::filesystem::remove(tempPath, ec);
    }
}

void notify(uint32_t slot, SaveEventFn SaveObserverRecord::* which, const char* what) {
    // Callbacks may unregister observers.
    const auto observers = s_observers;
    for (const auto& observer : observers) {
        if (!observer.mod->active || observer.*which == nullptr) {
            continue;
        }
        try {
            (observer.*which)(observer.mod->context.get(), slot, observer.userData);
        } catch (const std::exception& e) {
            fail_mod(*observer.mod, MOD_ERROR,
                fmt::format("exception in {} save callback: {}", what, e.what()));
        } catch (...) {
            fail_mod(*observer.mod, MOD_ERROR,
                fmt::format("unknown exception in {} save callback", what));
        }
    }
}

}  // namespace

void save_slot_new(uint32_t slot) {
    if (slot >= kSlotCount) {
        return;
    }
    load_sidecar();
    auto& store = s_slots[slot];
    store.mods.clear();
    store.snapshotValid = false;
    s_currentSlot = static_cast<int32_t>(slot);
    item_gives_clear();
    Log.info("new save in slot {}; mod blob store cleared", slot);
    notify(slot, &SaveObserverRecord::onNewSave, "new-save");
}

void save_slot_loaded(uint32_t slot, const void* slotData) {
    if (slot >= kSlotCount) {
        return;
    }
    load_sidecar();
    auto& store = s_slots[slot];
    if (store.snapshotValid && slotData != nullptr) {
        const auto crc = utils::crc32(slotData, kQuestLogSize);
        if (crc != store.snapshotCrc) {
            Log.warn("slot {} save data does not match the mod sidecar snapshot; mod save "
                     "data may be stale (card file changed externally?)",
                slot);
        }
    }
    s_currentSlot = static_cast<int32_t>(slot);
    item_gives_clear();
    notify(slot, &SaveObserverRecord::onLoaded, "save-loaded");
}

void save_slot_written(uint32_t slot, const void* slotData) {
    if (slot >= kSlotCount) {
        return;
    }
    s_currentSlot = static_cast<int32_t>(slot);
    notify(slot, &SaveObserverRecord::onWritten, "save-written");
    load_sidecar();
    auto& store = s_slots[slot];
    if (slotData != nullptr) {
        store.snapshotValid = true;
        store.snapshotCrc = utils::crc32(slotData, kQuestLogSize);
    }
    flush_sidecar();
}

void save_slot_copied(uint32_t fromSlot, uint32_t toSlot) {
    if (fromSlot >= kSlotCount || toSlot >= kSlotCount || fromSlot == toSlot) {
        return;
    }
    load_sidecar();
    s_slots[toSlot] = s_slots[fromSlot];
    flush_sidecar();
    Log.info("mod save data copied with slot {} -> {}", fromSlot, toSlot);
}

void save_slot_erased(uint32_t slot) {
    if (slot >= kSlotCount) {
        return;
    }
    load_sidecar();
    s_slots[slot] = SlotStore{};
    flush_sidecar();
    Log.info("mod save data erased with slot {}", slot);
}

void save_no_slot() {
    s_currentSlot = -1;
    item_gives_clear();
}

namespace {

BlobMap* current_blobs(const LoadedMod& mod, bool create) {
    if (s_currentSlot < 0) {
        return nullptr;
    }
    load_sidecar();
    auto& mods = s_slots[s_currentSlot].mods;
    if (!create) {
        const auto it = mods.find(mod.metadata.id);
        return it != mods.end() ? &it->second : nullptr;
    }
    return &mods[mod.metadata.id];
}

}  // namespace

ModResult save_set_blob(LoadedMod& mod, const char* name, const void* data, size_t size) {
    auto* blobs = current_blobs(mod, true);
    if (blobs == nullptr) {
        return MOD_UNAVAILABLE;
    }
    size_t total = size;
    for (const auto& [blobName, bytes] : *blobs) {
        if (blobName != name) {
            total += bytes.size();
        }
    }
    if (total > SAVE_BLOB_BUDGET_BYTES) {
        Log.error("[{}] save blob '{}' rejected: {} bytes would exceed the {}-byte budget",
            mod.metadata.id, name, total, SAVE_BLOB_BUDGET_BYTES);
        return MOD_UNAVAILABLE;
    }
    const auto* bytes = static_cast<const uint8_t*>(data);
    (*blobs)[name] = std::vector<uint8_t>{bytes, bytes + size};
    return MOD_OK;
}

ModResult save_get_blob(LoadedMod& mod, const char* name, void* buf, size_t& inoutSize) {
    auto* blobs = current_blobs(mod, false);
    if (blobs == nullptr) {
        return MOD_UNAVAILABLE;
    }
    const auto it = blobs->find(name);
    if (it == blobs->end()) {
        return MOD_UNAVAILABLE;
    }
    if (buf == nullptr) {
        inoutSize = it->second.size();
        return MOD_OK;
    }
    if (inoutSize < it->second.size()) {
        return MOD_INVALID_ARGUMENT;
    }
    std::memcpy(buf, it->second.data(), it->second.size());
    inoutSize = it->second.size();
    return MOD_OK;
}

ModResult save_delete_blob(LoadedMod& mod, const char* name) {
    auto* blobs = current_blobs(mod, false);
    if (blobs == nullptr) {
        return MOD_UNAVAILABLE;
    }
    return blobs->erase(name) != 0 ? MOD_OK : MOD_INVALID_ARGUMENT;
}

ModResult save_observe(LoadedMod& mod, SaveEventFn onNewSave, SaveEventFn onLoaded,
    SaveEventFn onWritten, void* userData, uint64_t& outHandle) {
    auto& observer = s_observers.emplace_back();
    observer.handle = s_nextHandle++;
    observer.mod = &mod;
    observer.onNewSave = onNewSave;
    observer.onLoaded = onLoaded;
    observer.onWritten = onWritten;
    observer.userData = userData;
    outHandle = observer.handle;
    return MOD_OK;
}

ModResult save_unobserve(LoadedMod& mod, uint64_t handle) {
    const auto removed = std::erase_if(s_observers,
        [&](const auto& observer) { return observer.handle == handle && observer.mod == &mod; });
    return removed != 0 ? MOD_OK : MOD_INVALID_ARGUMENT;
}

ModResult save_peek_blob(
    LoadedMod& mod, uint32_t slot, const char* name, void* buf, size_t& inoutSize) {
    if (slot >= kSlotCount) {
        return MOD_INVALID_ARGUMENT;
    }
    load_sidecar();
    const auto& mods = s_slots[slot].mods;
    const auto modIt = mods.find(mod.metadata.id);
    if (modIt == mods.end()) {
        return MOD_UNAVAILABLE;
    }
    const auto it = modIt->second.find(name);
    if (it == modIt->second.end()) {
        return MOD_UNAVAILABLE;
    }
    if (buf == nullptr) {
        inoutSize = it->second.size();
        return MOD_OK;
    }
    if (inoutSize < it->second.size()) {
        return MOD_INVALID_ARGUMENT;
    }
    std::memcpy(buf, it->second.data(), it->second.size());
    inoutSize = it->second.size();
    return MOD_OK;
}

void save_remove_mod(LoadedMod& mod) {
    std::erase_if(s_observers, [&](const auto& observer) { return observer.mod == &mod; });
    // Blob data persists across mod reloads.
}

namespace {
bool is_valid_blob_name(const char* name) {
    if (name == nullptr) {
        return false;
    }
    const std::string_view view{name};
    return !view.empty() && view.size() <= kMaxBlobNameLength;
}

ModResult save_set_blob_(ModContext* context, const char* name, const void* data, size_t size) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || !is_valid_blob_name(name) || (data == nullptr && size != 0) ||
        size > SAVE_BLOB_BUDGET_BYTES)
    {
        return MOD_INVALID_ARGUMENT;
    }
    return save_set_blob(*mod, name, data, size);
}

ModResult save_get_blob_(ModContext* context, const char* name, void* buf, size_t* inoutSize) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || !is_valid_blob_name(name) || inoutSize == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return save_get_blob(*mod, name, buf, *inoutSize);
}

ModResult save_delete_blob_(ModContext* context, const char* name) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || !is_valid_blob_name(name)) {
        return MOD_INVALID_ARGUMENT;
    }
    return save_delete_blob(*mod, name);
}

ModResult save_observe_saves_(ModContext* context, SaveEventFn onNewSave, SaveEventFn onLoaded,
    SaveEventFn onWritten, void* userData, SaveObserverHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || (onNewSave == nullptr && onLoaded == nullptr && onWritten == nullptr)) {
        return MOD_INVALID_ARGUMENT;
    }
    uint64_t handle = 0;
    const auto result = save_observe(*mod, onNewSave, onLoaded, onWritten, userData, handle);
    if (outHandle != nullptr) {
        *outHandle = handle;
    }
    return result;
}

ModResult save_unobserve_saves_(ModContext* context, SaveObserverHandle handle) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || handle == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    return save_unobserve(*mod, handle);
}

ModResult save_peek_blob_(
    ModContext* context, uint32_t slot, const char* name, void* buf, size_t* inoutSize) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || !is_valid_blob_name(name) || inoutSize == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return save_peek_blob(*mod, slot, name, buf, *inoutSize);
}

constexpr SaveService s_saveService{
    .header = SERVICE_HEADER(SaveService, SAVE_SERVICE_MAJOR, SAVE_SERVICE_MINOR),
    .set_blob = save_set_blob_,
    .get_blob = save_get_blob_,
    .delete_blob = save_delete_blob_,
    .observe_saves = save_observe_saves_,
    .unobserve_saves = save_unobserve_saves_,
    .peek_blob = save_peek_blob_,
};

}  // namespace

constinit const ServiceModule g_saveModule{
    .id = SAVE_SERVICE_ID,
    .majorVersion = SAVE_SERVICE_MAJOR,
    .minorVersion = SAVE_SERVICE_MINOR,
    .service = &s_saveService,
    .modDetached = save_remove_mod,
};

}  // namespace dusk::mods::svc
