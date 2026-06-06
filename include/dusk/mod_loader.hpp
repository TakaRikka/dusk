#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <ranges>

#include "dusk/mod_api.h"
#include "dusk/config_var.hpp"

namespace dusk::modding {
class ModBundle;
class NativeModule;
}

namespace dusk {

struct RmlTabContentCallback {
    void (*build_fn)(void* panel, void* userdata);
    void* userdata;
};

struct RmlTabUpdateCallback {
    void (*update_fn)(void* userdata);
    void* userdata;
};

struct ModMetadata {
    std::string id;
    std::string name;
    std::string version;
    std::string author;
    std::string description;
    bool hasCode;
};

enum class SettingType { Bool, Int, Float };

// A mod's declared setting plus its live value. Built from the DuskSetting a mod
// passes to define_settings, with the saved value overlaid from config.json.
struct ModSetting {
    std::string key;
    std::string label;
    std::string help;
    SettingType type = SettingType::Bool;
    double defaultValue = 0.0;
    double minValue = 0.0;
    double maxValue = 0.0;
    double step = 0.0;
    double value = 0.0;
};

struct NativeMod {
    std::unique_ptr<modding::NativeModule> handle;
    DuskModAPI api{};

    using FnInit = void (*)(DuskModAPI*);
    using FnTick = void (*)(DuskModAPI*);
    using FnCleanup = void (*)(DuskModAPI*);

    FnInit fn_init = nullptr;
    FnTick fn_tick = nullptr;
    FnCleanup fn_cleanup = nullptr;
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
     * Mod does not have a native library suitable for this build's platform.
     */
    ModMissingPlatform,

    /**
     * Mod is built for a different API version than this build of the game.
     */
    ApiVersionMismatch,

    /**
     * Unknown error loading the native mod.
     */
    Unknown,
};

struct LoadedMod {
    ModMetadata metadata;
    std::string mod_path;
    std::string dir;

    // Enabled state is owned by the game (persisted as a CVar), not the mod.
    // `enabled` is the live mirror of cvarIsEnabled.
    std::unique_ptr<ConfigVar<bool>> cvarIsEnabled;
    bool enabled = true;
    bool fromDir = true;       // bundle source kind (dir vs .dusk), for hot-reload
    bool active = false;       // currently loaded + initialized
    bool load_failed = false;

    std::string source_lib;                       // on-disk file to watch for hot-reload
    std::filesystem::file_time_type lib_mtime{};  // its mtime when last (re)loaded

    std::vector<ModSetting> settings;

    NativeModStatus native_status = NativeModStatus::None;
    std::unique_ptr<NativeMod> native;

    std::unique_ptr<modding::ModBundle> bundle;

    std::vector<RmlTabContentCallback> tab_content;
    std::vector<RmlTabUpdateCallback> tab_updates;
};

class ModLoader {
public:
    static ModLoader& instance();

    void setModsDir(std::filesystem::path dir) { m_modsDir = std::move(dir); }
    void init();
    void tick();
    void shutdown();

    // Hot-reload: cheap per-frame check that reloads any mod whose library file
    // changed on disk (driven by a background file watcher). Called from tick().
    void update();

    // Rescan the mods directory and load any newly-added mods (additive).
    void refresh();

    [[nodiscard]] auto mods() const {
        return m_mods | std::views::transform([](const auto& m) -> LoadedMod& { return *m; });
    }

    [[nodiscard]] auto active_mods() const {
        return mods() | std::views::filter([](const auto& m) { return m.active; });
    }

    [[nodiscard]] const std::filesystem::path& modsDir() const { return m_modsDir; }

    // Lookup + live enable/disable + settings, addressed by mod id (used by UI).
    LoadedMod* find(std::string_view id);
    [[nodiscard]] bool isEnabled(std::string_view id);
    void setEnabled(std::string_view id, bool enabled);
    [[nodiscard]] double getSetting(std::string_view id, std::string_view key);
    void setSetting(std::string_view id, std::string_view key, double value);

    // Called by the mod API on behalf of the calling (current) mod.
    void modDefineSettings(LoadedMod& mod, const DuskSetting* arr, uint32_t count);
    [[nodiscard]] double modGetSetting(LoadedMod& mod, const char* key);
    void modSetSetting(LoadedMod& mod, const char* key, double value);

private:
    std::vector<std::unique_ptr<LoadedMod>> m_mods;
    std::filesystem::path m_modsDir;
    bool m_initialized = false;

    void tryLoadDusk(const std::filesystem::path& modPath, bool fromDir);
    void tryLoadNativeMod(LoadedMod& mod);
    void buildAPI(LoadedMod& mod);
    void initOverlayFiles();

    bool initMod(LoadedMod& mod);   // buildAPI + fn_init (native already loaded)
    void unloadMod(LoadedMod& mod); // clear hooks + fn_cleanup + unload native lib
    void reloadMod(LoadedMod& mod); // hot-reload a single mod in place

    [[nodiscard]] std::filesystem::path configPath(const LoadedMod& mod) const;
    [[nodiscard]] std::filesystem::path schemaPath(const LoadedMod& mod) const;
    void readConfig(LoadedMod& mod);  // settings values only (enabled is a CVar)
    void writeConfig(const LoadedMod& mod);
    void writeSchema(const LoadedMod& mod);
    void parseSchema(LoadedMod& mod);
    void applySetting(LoadedMod& mod, std::string_view key, double value);
};

using ModIndex = std::ranges::range_difference_t<decltype(std::declval<ModLoader>().mods())>;

}  // namespace dusk
