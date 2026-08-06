#include "registry.hpp"
#include "slot_map.hpp"

#include <borealis/log.hpp>
#include "aurora/dvd.h"
#include "dusk/mods/loader/loader.hpp"
#include "mods/svc/overlay.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <mutex>
#include <string_view>
#include <unordered_map>
#include <utility>

using namespace std::string_literals;

namespace dusk::mods::svc {
namespace {

constexpr borealis::Log Log{"dusk::mods::overlay"};

struct OverlayFileData {
    std::string bundlePath;
    std::shared_ptr<ModBundle> bundle;
    std::shared_ptr<const std::vector<u8>> buffer;
};

// Keyed by the id passed to Aurora as per-file userdata. Guarded by s_overlayMutex: Aurora may
// call cbOpen from a DVD thread while the game thread replaces the set in overlay_sync_files.
// The shared bundle/buffer pointer keeps a disabled/reloaded mod's data readable until the last
// open completes.
std::unordered_map<uintptr_t, OverlayFileData> s_overlayFiles;
uintptr_t s_nextOverlayId = 1;
std::mutex s_overlayMutex;

struct OverlayArcData {
    std::string fullFilePath;
    std::shared_ptr<ModBundle> bundle;  
    std::shared_ptr<const std::vector<u8>> buffer; // Only used by runtime buffer overlays
};
struct OverlayedArc {
    std::unordered_map<std::string, OverlayArcData> pathInArcToData;
};
std::unordered_map<s32, OverlayedArc> s_arcOverlays;

struct RuntimeOverlaySlot {
    std::string discPath;
    std::string bundlePath;                         // bundle-backed if non-empty
    std::shared_ptr<const std::vector<u8>> buffer;  // buffer-backed otherwise
    size_t size = 0;
    uint64_t order = 0;
};
SlotMap<RuntimeOverlaySlot> s_runtimeOverlays;
uint64_t s_nextRuntimeOrder = 0;
bool s_overlaysDirty = false;

// Aurora matches overlay paths against the disc case-insensitively and later entries win, so
// claims are tracked by lowercased path and re-claims by a different mod warn.
void claim_overlay_path(std::unordered_map<std::string, const LoadedMod*>& claims,
    const std::string& discPath, const LoadedMod& mod) {
    std::string key = discPath;
    for (auto& ch : key) {
        if (ch >= 'A' && ch <= 'Z') {
            ch += 'a' - 'A';
        }
    }
    const auto [it, inserted] = claims.try_emplace(std::move(key), &mod);
    if (!inserted && it->second != &mod) {
        Log.warn("Overlay conflict: '{}' is provided by both '{}' and '{}'; '{}' wins.", discPath,
            it->second->metadata.id, mod.metadata.id, mod.metadata.id);
        it->second = &mod;
    }
}

void claim_arc_overlay_path(LoadedMod& mod, const std::string& filePath, const std::string& dvdPath,
    const std::string& pathInArc, std::shared_ptr<const std::vector<u8>> buffer = nullptr) {

    s32 entryNum = DVDConvertPathToEntrynum(dvdPath.c_str());
    if (entryNum == -1) {
        // The arc we're attempting to overlay doesn't exist on the DVD, ignore the file instead.
        return;
    }
    // Log.info("{}", entryNum);
    auto it = s_arcOverlays.find(entryNum);
    if (it == s_arcOverlays.end()) {
        s_arcOverlays[entryNum] = {};
        it = s_arcOverlays.find(entryNum);
    }

    OverlayedArc& arc = it->second;

    arc.pathInArcToData[pathInArc] = OverlayArcData{
        .fullFilePath = filePath,
        .bundle = mod.bundle,
        .buffer = buffer
    };
}

void find_overlay_files(std::vector<AuroraOverlayFile>& files, LoadedMod& mod,
    std::unordered_map<std::string, const LoadedMod*>& claims) {
    for (const auto& file : mod.bundle->getFileNames()) {
        if (!file.starts_with("overlay/")) {
            continue;
        }

        auto overlayPath = file.substr("overlay/"s.size());
        assert(!overlayPath.starts_with('/'));
        overlayPath.insert(0, "/");

        size_t pos = overlayPath.find(".arc/");
        if (pos != std::string::npos) {
            // All arc overlays that are directories
            claim_arc_overlay_path(
                mod, file, overlayPath.substr(0, pos + 4), overlayPath.substr(pos + 5));
            continue;
        }

        // Register the overlay as a file
        const auto size = mod.bundle->getFileSize(file);

        const auto id = s_nextOverlayId++;
        s_overlayFiles.emplace(id, OverlayFileData{file, mod.bundle, nullptr});
        claim_overlay_path(claims, overlayPath, mod);
        files.emplace_back(strdup(overlayPath.c_str()), reinterpret_cast<void*>(id), size);
    }
}

void append_runtime_overlays(std::vector<AuroraOverlayFile>& files, LoadedMod& mod,
    std::unordered_map<std::string, const LoadedMod*>& claims) {
    // Aurora resolves duplicate paths later-entry-wins, so emit in registration order (SlotMap
    // iteration is index order, and freed indices are reused).
    std::vector<const RuntimeOverlaySlot*> slots;
    s_runtimeOverlays.for_each([&](uint64_t, const auto& entry) {
        if (entry.owner == &mod) {
            slots.push_back(&entry.value);
        }
    });
    std::ranges::sort(slots, {}, &RuntimeOverlaySlot::order);

    for (const auto* slot : slots) {
        const auto id = s_nextOverlayId++;

        // Claim this as an arc overlay if it is one.
        size_t arcDirPos = std::string(slot->discPath).find(".arc/");
        if (arcDirPos != std::string::npos) {
            claim_arc_overlay_path(mod, slot->bundlePath, slot->discPath.substr(0, arcDirPos + 4), slot->discPath.substr(arcDirPos + 5), slot->buffer);
            continue;
        }

        if (slot->buffer != nullptr) {
            s_overlayFiles.emplace(id, OverlayFileData{{}, nullptr, slot->buffer});
        } else {
            s_overlayFiles.emplace(id, OverlayFileData{slot->bundlePath, mod.bundle, nullptr});
        }
        claim_overlay_path(claims, slot->discPath, mod);
        files.emplace_back(strdup(slot->discPath.c_str()), reinterpret_cast<void*>(id), slot->size);
    }
}

struct OpenOverlayFile {
    std::vector<u8> ownedData;
    std::shared_ptr<const std::vector<u8>> shared;
    size_t pos = 0;

    [[nodiscard]] const std::vector<u8>& data() const {
        return shared != nullptr ? *shared : ownedData;
    }
};

void* cbOpen(void* userdata) {
    const auto id = reinterpret_cast<uintptr_t>(userdata);
    OverlayFileData fileData;
    {
        std::lock_guard lock{s_overlayMutex};
        const auto it = s_overlayFiles.find(id);
        if (it == s_overlayFiles.end()) {
            // The overlay set was re-pushed between the FST lookup and this call.
            return nullptr;
        }
        fileData = it->second;
    }

    if (fileData.buffer != nullptr) {
        return new OpenOverlayFile{.shared = std::move(fileData.buffer)};
    }

    try {
        auto fileContents = fileData.bundle->readFile(fileData.bundlePath);
        return new OpenOverlayFile{.ownedData = std::move(fileContents)};
    } catch (const std::runtime_error& e) {
        Log.error("Failed to read overlay file {}: {}", fileData.bundlePath, e.what());
        return nullptr;
    }
}

void cbClose(void* handle) {
    const auto openFile = static_cast<OpenOverlayFile*>(handle);
    delete openFile;
}

int64_t cbRead(void* handle, uint8_t* buf, const size_t len) {
    auto& openFile = *static_cast<OpenOverlayFile*>(handle);

    const auto remainingSpace = openFile.data().size() - openFile.pos;
    const auto toRead = std::min(remainingSpace, len);
    std::memcpy(buf, openFile.data().data() + openFile.pos, toRead);
    openFile.pos += toRead;
    return static_cast<int64_t>(toRead);
}

int64_t cbSeek(void* handle, int64_t offset, int32_t whence) {
    if (whence != 0) {
        Log.fatal("Invalid seek mode from aurora: {}", whence);
    }

    auto& openFile = *static_cast<OpenOverlayFile*>(handle);
    const auto posSigned =
        std::clamp(offset, static_cast<int64_t>(0), static_cast<int64_t>(openFile.data().size()));
    openFile.pos = static_cast<size_t>(posSigned);
    return posSigned;
}

constexpr AuroraOverlayCallbacks s_overlayCallbacks = {
    .open = cbOpen,
    .close = cbClose,
    .read = cbRead,
    .seek = cbSeek,
};

void overlay_sync_files() {
    static bool callbacksRegistered = false;
    if (!callbacksRegistered) {
        aurora_dvd_overlay_callbacks(&s_overlayCallbacks);
        callbacksRegistered = true;
    }

    s_overlaysDirty = false;

    std::vector<AuroraOverlayFile> files;
    std::unordered_map<std::string, const LoadedMod*> claims;
    {
        std::lock_guard lock{s_overlayMutex};
        s_overlayFiles.clear();
        s_arcOverlays.clear();
        for (auto& mod : ModLoader::instance().active_mods()) {
            find_overlay_files(files, mod, claims);
            append_runtime_overlays(files, mod, claims);
        }
    }

    Log.debug("Registering {} overlay file(s).", files.size());
    aurora_dvd_overlay_files(files.data(), files.size(), nullptr);

    for (const auto& file : files) {
        std::free(const_cast<char*>(file.fileName));
    }
}

uint64_t overlay_add_file(
    LoadedMod& mod, std::string discPath, std::string bundlePath, size_t size) {
    const auto handle = s_runtimeOverlays.emplace(mod, RuntimeOverlaySlot{
                                                           .discPath = std::move(discPath),
                                                           .bundlePath = std::move(bundlePath),
                                                           .size = size,
                                                           .order = s_nextRuntimeOrder++,
                                                       });
    s_overlaysDirty = true;
    return handle;
}

uint64_t overlay_add_buffer(LoadedMod& mod, std::string discPath, std::vector<u8> data) {
    const auto size = data.size();
    const auto handle = s_runtimeOverlays.emplace(
        mod, RuntimeOverlaySlot{
                 .discPath = std::move(discPath),
                 .buffer = std::make_shared<const std::vector<u8>>(std::move(data)),
                 .size = size,
                 .order = s_nextRuntimeOrder++,
             });
    s_overlaysDirty = true;
    return handle;
}

bool overlay_remove(LoadedMod& mod, uint64_t handle) {
    if (!s_runtimeOverlays.erase_owned(handle, mod)) {
        return false;
    }
    s_overlaysDirty = true;
    return true;
}

void overlay_remove_mod(LoadedMod& mod) {
    if (s_runtimeOverlays.erase_all(mod) != 0) {
        s_overlaysDirty = true;
    }
}

bool consume_overlays_dirty() {
    return std::exchange(s_overlaysDirty, false);
}

constexpr size_t kMaxOverlayFileSize = UINT32_MAX;

bool is_valid_disc_path(const char* discPath) {
    if (discPath == nullptr) {
        return false;
    }
    const std::string_view path{discPath};
    return path.starts_with('/') && is_safe_resource_path(path.substr(1));
}

ModResult overlay_add_file(
    ModContext* context, const char* discPath, const char* bundlePath, OverlayHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || !is_valid_disc_path(discPath) || bundlePath == nullptr ||
        !is_safe_resource_path(bundlePath))
    {
        return MOD_INVALID_ARGUMENT;
    }

    size_t size = 0;
    try {
        size = mod->bundle->getFileSize(bundlePath);
    } catch (const std::exception& e) {
        Log.error("[{}] overlay add_file '{}' failed: {}", mod->metadata.id, bundlePath, e.what());
        return MOD_UNAVAILABLE;
    }
    if (size > kMaxOverlayFileSize) {
        Log.error("[{}] overlay add_file '{}' failed: file too large ({} bytes)", mod->metadata.id,
            bundlePath, size);
        return MOD_INVALID_ARGUMENT;
    }

    const auto handle = overlay_add_file(*mod, discPath, bundlePath, size);
    if (outHandle != nullptr) {
        *outHandle = handle;
    }
    return MOD_OK;
}

ModResult overlay_add_buffer(ModContext* context, const char* discPath, const void* data,
    size_t size, OverlayHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || !is_valid_disc_path(discPath) || (data == nullptr && size != 0) ||
        size > kMaxOverlayFileSize)
    {
        return MOD_INVALID_ARGUMENT;
    }

    const auto* bytes = static_cast<const u8*>(data);
    const auto handle = overlay_add_buffer(*mod, discPath, std::vector<u8>{bytes, bytes + size});
    if (outHandle != nullptr) {
        *outHandle = handle;
    }
    return MOD_OK;
}

ModResult overlay_remove(ModContext* context, OverlayHandle handle) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || handle == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    if (!overlay_remove(*mod, handle)) {
        Log.error("[{}] overlay remove failed: unknown handle {}", mod->metadata.id, handle);
        return MOD_INVALID_ARGUMENT;
    }
    return MOD_OK;
}

bool arc_has_overlay(s32 dvdEntryNum) {
    if (s_arcOverlays.find(dvdEntryNum) != s_arcOverlays.end()) {
        return true;
    }
    return false;
}

std::shared_ptr<const std::vector<u8>> get_arc_overlay_buffer(
    s32 dvdEntryNum, const std::string& pathInArc) {
    const auto& arcIt = s_arcOverlays.find(dvdEntryNum);
    if (arcIt == s_arcOverlays.end()) {
        return nullptr;
    }

    const auto& arcFileIt = arcIt->second.pathInArcToData.find(pathInArc);
    if (arcFileIt == arcIt->second.pathInArcToData.end()) {
        return nullptr;
    }

    if (arcFileIt->second.buffer != nullptr) {
        // If we are using a buffer overlay, return the shared buffer ptr
        return arcFileIt->second.buffer;
    }

    // Load the file as a unique pointer, which will be owned as the arc
    return std::move(std::make_shared<const std::vector<u8>>(arcFileIt->second.bundle->readFile(arcFileIt->second.fullFilePath)));
}

void overlay_init() {
    JKRArchive::sArcHasOverlayFn = arc_has_overlay;
    JKRArchive::sGetArcOverlayBufferFn = get_arc_overlay_buffer;
}

constexpr OverlayService s_overlayService{
    .header = SERVICE_HEADER(OverlayService, OVERLAY_SERVICE_MAJOR, OVERLAY_SERVICE_MINOR),
    .add_file = overlay_add_file,
    .add_buffer = overlay_add_buffer,
    .remove = overlay_remove,
};

}  // namespace

constinit const ServiceModule g_overlayModule{
    .id = OVERLAY_SERVICE_ID,
    .majorVersion = OVERLAY_SERVICE_MAJOR,
    .minorVersion = OVERLAY_SERVICE_MINOR,
    .service = &s_overlayService,
    .initialize = &overlay_init,
    .modDetached = overlay_remove_mod,
    .lifecycleApplied = overlay_sync_files,
    .frameEnd =
        [] {
            if (consume_overlays_dirty()) {
                overlay_sync_files();
            }
        },
};
}  // namespace dusk::mods::svc
