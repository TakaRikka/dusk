#pragma once

#include <filesystem>
#include <memory>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include "dusk/config.hpp"
#include "dusk/config_var.hpp"
#include "mods/api.h"

namespace dusk::mods {
struct LoadedMod;
class ModBundle;
}  // namespace dusk::mods

struct ModContext {
    dusk::mods::LoadedMod* mod = nullptr;
};

namespace dusk::mods::loader {
class NativeModule;
}  // namespace dusk::mods::loader

namespace dusk::mods {

struct ModDependencyEdge {
    LoadedMod* mod = nullptr;
    bool required = false;
};

struct ModManifestInfo {
    struct Import {
        std::string id;
        uint16_t major = 0;
        bool required = false;
        bool operator==(const Import&) const = default;
    };
    struct Export {
        std::string id;
        uint16_t major = 0;
        bool operator==(const Export&) const = default;
    };
    std::vector<Import> imports;
    std::vector<Export> exports;
    bool operator==(const ModManifestInfo&) const = default;
};

struct ModMetadata {
    std::string id;
    std::string name;
    std::string version;
    std::string author;
    std::string description;
    std::string iconPath;
    std::string bannerPath;
};

struct ModSearchDir {
    std::filesystem::path path;
    // Directory bundles dlopen their native lib in place instead of extracting it to the cache.
    // Required where extracted code cannot run (iOS), desirable for signed/read-only installs.
    bool inPlaceNative = false;
    // Native library location for platforms that restrict placement (e.g. iOS/tvOS Frameworks/)
    std::filesystem::path nativeLibDir;
};

struct ModMetaParsed {
    uint32_t abiVersion = 0;
    std::vector<ModMetaImport*> imports;
    std::vector<ModMetaExport*> exports;
    std::vector<ModMetaHookFn*> hookFns;
    std::vector<ModMetaHookMem*> hookMems;
    std::vector<ModMetaHookMemExt*> hookMemExts;
    std::vector<ModMetaHookName*> hookNames;
};

inline const char* hook_mem_vtable_symbol(const ModMetaHookMem& rec) {
    return reinterpret_cast<const char*>(&rec) + sizeof(ModMetaHookMem);
}

inline const char* hook_mem_display_name(const ModMetaHookMem& rec) {
    const char* vtable = hook_mem_vtable_symbol(rec);
    return vtable + std::char_traits<char>::length(vtable) + 1;
}

inline const char* hook_mem_vtable_symbol(const ModMetaHookMemExt& rec) {
    return reinterpret_cast<const char*>(&rec) + sizeof(ModMetaHookMemExt);
}

inline const char* hook_mem_display_name(const ModMetaHookMemExt& rec) {
    const char* vtable = hook_mem_vtable_symbol(rec);
    return vtable + std::char_traits<char>::length(vtable) + 1;
}

inline const char* hook_name_symbol(const ModMetaHookName& rec) {
    return reinterpret_cast<const char*>(&rec) + sizeof(ModMetaHookName);
}

struct NativeMod {
    std::unique_ptr<loader::NativeModule> handle;
    const ModMeta* meta = nullptr;
    ModMetaParsed parsed;
    ModContext** contextSymbol = nullptr;

    ModInitializeFn fn_initialize = nullptr;
    ModUpdateFn fn_update = nullptr;
    ModShutdownFn fn_shutdown = nullptr;
};

enum class NativeModStatus : u8 {
    /**
     * Mod does not have native code included.
     */
    None,

    /**
     * Native code mod loaded successfully.
     *
     * Note that this only indicates load status of the native library. If the native lib throws in
     * its init function, it will still be disabled!
     */
    Loaded,

    /**
     * This build was compiled without native mod support!
     */
    BuildDisabled,

    /**
     * Mod ships native libraries, but none matches this build's platform and architecture.
     */
    ModMissingPlatform,

    /**
     * Mod is built for a different ABI version than this build of the game.
     */
    ApiVersionMismatch,

    /**
     * Mod is missing a required native API export.
     */
    MissingExport,

    /**
     * Mod's metadata record section is malformed.
     */
    InvalidMetadata,

    /**
     * Mod bundle contains files in an invalid location.
     */
    InvalidBundle,

    /**
     * Unknown error loading the native mod.
     */
    Unknown,
};

struct LoadedMod {
    ModMetadata metadata;
    std::filesystem::path modPath;
    std::filesystem::path dir;
    // Stable UTF-8 storage for HostService::mod_dir.
    std::string dirUtf8;

    uint32_t searchDirIndex = 0;
    // Native lib is dlopen'd in place and stays resident for the session. Reload is unsupported.
    bool inPlace = false;

    std::unique_ptr<ConfigVar<bool>> cvarIsEnabled;
    config::Subscription enabledSubscription = 0;

    bool active = false;
    bool loadFailed = false;
    std::string failureReason;

    // mod_initialize succeeded; a mod_shutdown is owed on deactivation.
    bool initialized = false;
    // Static service exports are currently present in the registry.
    bool servicesRegistered = false;
    // Lifecycle state last applied by the loader; diffed against cvarIsEnabled to pick up
    // runtime enable/disable requests.
    bool enabledApplied = false;
    // Deactivated because a provider it imports from was disabled, not by its own cvar.
    bool suspendedByProvider = false;
    // Bumped per native lib extraction so every dlopen sees a fresh path (and thus a fresh
    // image with fresh statics; a previous dlclose may not fully unmap). Also bumped by
    // asset-only reloads, so it doubles as a generation for anything caching per-mod content.
    uint32_t cacheGeneration = 0;
    // Currently extracted native library, empty if none.
    std::filesystem::path nativePath;
    // Read-only directory containing the current platform's main module and runtime libraries.
    std::filesystem::path nativeDir;
    // Stable UTF-8 storage for HostService::native_dir.
    std::string nativeDirUtf8;

    NativeModStatus nativeStatus = NativeModStatus::None;
    std::unique_ptr<NativeMod> native;
    std::unique_ptr<ModContext> context;

    // Shared with overlay file registrations so in-flight DVD reads survive disable/reload.
    std::shared_ptr<ModBundle> bundle;

    ModManifestInfo manifestInfo;

    // Mods this mod imports services from, and mods importing services from this mod.
    std::vector<ModDependencyEdge> dependencies;
    std::vector<ModDependencyEdge> dependents;
};

class ModLoader {
public:
    static ModLoader& instance();

    void set_search_dirs(std::vector<ModSearchDir> dirs) { m_searchDirs = std::move(dirs); }
    void set_cache_dir(std::filesystem::path dir) { m_cacheDir = std::move(dir); }
    void init();
    void tick();
    void shutdown();

    void request_enable(std::string_view id);
    void request_disable(std::string_view id);
    void request_reload(std::string_view id);
    void notify_mod_failure(LoadedMod& mod, bool firstFailure);

    [[nodiscard]] auto mods() const {
        return m_mods | std::views::transform([](const auto& m) -> LoadedMod& { return *m; });
    }

    [[nodiscard]] auto active_mods() const {
        return mods() | std::views::filter([](const auto& m) { return m.active; });
    }

private:
    enum class RequestKind : u8 { Enable, Disable, Reload };
    struct Request {
        std::string modId;
        RequestKind kind;
    };
    // ModLoader::tick runs inside fapGm_Execute, so code from an unloading mod can still be
    // live on the stack (its frame unwinds after the tick). dlclose is therefore deferred to
    // the next tick, by which point every per-frame entry into the mod should have returned.
    struct RetiredNative {
        std::unique_ptr<NativeMod> native;
        std::filesystem::path directory;
    };

    std::vector<std::unique_ptr<LoadedMod>> m_mods;
    std::vector<ModSearchDir> m_searchDirs;
    std::filesystem::path m_cacheDir;
    std::vector<Request> m_pendingRequests;
    std::vector<std::string> m_pendingFailures;
    std::vector<RetiredNative> m_retiredNatives;
    bool m_initialized = false;
    bool m_startupComplete = false;

    void try_load_mod(const std::filesystem::path& modPath, bool fromDir, uint32_t searchDirIndex);
    void load_native(LoadedMod& mod, const std::string& dllEntry,
        const std::vector<std::string>& runtimeEntries);
    bool load_native_if_present(LoadedMod& mod);
    // Resolved <nativeLibDir>/<mod id><ext> if it exists on disk, empty otherwise.
    [[nodiscard]] std::filesystem::path external_native_lib_path(const LoadedMod& mod) const;
    void unload_native(LoadedMod& mod);
    // Registers exports (if needed), resolves imports and runs mod_initialize.
    // Returns whether the mod ended up active; failures go through fail_mod.
    bool activate_mod(LoadedMod& mod);
    // Runs mod_shutdown (if needed), detaches the mod from every service, and unloads the
    // native lib. Must only run with no mod code on the stack (startup, shutdown, or top of tick).
    void deactivate_mod(LoadedMod& mod);
    void init_services();
    bool register_static_service_exports(LoadedMod& mod);
    bool resolve_service_imports(LoadedMod& mod);
    [[nodiscard]] std::string describe_missing_import(
        const char* serviceId, uint16_t majorVersion, uint16_t minMinorVersion) const;

    LoadedMod* find_mod(std::string_view id) const;
    void drain_retired_natives();
    void apply_pending_requests();
    void flush_toasts();
    void on_enabled_changed(LoadedMod& mod);
    // Deactivates `target` (if needed) and its transitive dependents, optionally re-reads the
    // bundle from disk, then reactivates whatever the current cvar/provider state allows.
    void apply_lifecycle_change(LoadedMod& target, bool reload);
    // `target` plus transitive active/suspended dependents, in m_mods (init) order.
    std::vector<LoadedMod*> collect_lifecycle_set(LoadedMod& target);
    bool reload_bundle(LoadedMod& mod);
    bool ensure_native_loaded(LoadedMod& mod);
};

using ModIndex = std::ranges::range_difference_t<decltype(std::declval<ModLoader>().mods())>;

}  // namespace dusk::mods
