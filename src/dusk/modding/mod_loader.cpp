#include "dusk/mod_loader.hpp"
#include "dusk/hook_system.hpp"
#include "dusk/logging.h"
#include "mod_loader.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <filesystem>
#include <memory>
#include <fstream>
#include <set>
#include <string>
#include <thread>
#include <unordered_map>

#include "aurora/dvd.h"
#include "dusk/config.hpp"
#include "dusk/io.hpp"
#include "dusk/ui/ui.hpp"
#include "miniz.h"
#include "native_module.hpp"
#include "nlohmann/json.hpp"

#if defined(__linux__)
#include <poll.h>
#include <sys/inotify.h>
#include <unistd.h>
#elif defined(__APPLE__)
#include <cerrno>
#include <fcntl.h>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

static aurora::Module Log("dusk::modLoader");

using namespace dusk::modding;
using namespace std::string_literals;
using namespace std::string_view_literals;
using json = nlohmann::json;
namespace fs = std::filesystem;

#if defined(_M_ARM64) || defined(__aarch64__)
static constexpr std::string_view k_archSuffix = "_arm64"sv;
#elif defined(_M_X64) || defined(__x86_64__)
static constexpr std::string_view k_archSuffix = "_x64"sv;
#elif defined(_M_IX86) || defined(__i386__)
static constexpr std::string_view k_archSuffix = "_x86"sv;
#else
static constexpr std::string_view k_archSuffix = ""sv;
#endif

static dusk::ModLoader g_modLoader;

// True only while a mod's mod_init() runs, so a mod touching its settings during
// init can't persist transient state to config.json.
static bool g_loadingMod = false;

// A mod's enabled state is a game-owned CVar (so the game manages + saves it,
// not the mod). CVars can't be deleted until full shutdown, so orphan them.
static std::vector<std::unique_ptr<dusk::ConfigVarBase>> OrphanedConfigVars;

// The enabled CVar key embeds the mod id verbatim (ids are validated to a safe
// charset, so no escaping/mangling is needed).
static std::string modEnabledCVarName(std::string_view id) {
    return fmt::format("mod.{}.enabled", id);
}

// On-screen notification (uses the game's toast overlay) so mod reloads/errors
// are visible without watching the log.
static void modToast(const std::string& title, const std::string& content, int seconds) {
    dusk::ui::push_toast({
        .type = "menu-notification",
        .title = title,
        .content = content,
        .duration = std::chrono::seconds(seconds),
    });
}

// ---- hot-reload file watcher -----------------------------------------------
// A background thread waits on OS file-change events for the mods directory and
// just raises g_libsChanged. ModLoader::update() (game thread) reacts: dlopen/
// dlclose and hook patching must run on the game thread, not the watcher thread.

static std::atomic<bool> g_libsChanged{false};
static std::thread g_watchThread;
static std::atomic<bool> g_watchStop{false};

#if defined(__linux__)
static int g_stopPipe[2] = {-1, -1};

static void watch_thread(std::string root) {
    const int fd = inotify_init1(0);
    if (fd < 0) {
        return;
    }
    std::unordered_map<int, std::string> watched;
    const auto addWatch = [&](const std::string& dir) {
        const int wd = inotify_add_watch(fd, dir.c_str(), IN_CLOSE_WRITE | IN_MOVED_TO | IN_CREATE);
        if (wd >= 0) {
            watched[wd] = dir;
        }
    };
    std::error_code ec;
    if (fs::exists(root, ec)) {
        addWatch(root);
        for (const auto& e : fs::directory_iterator(root, ec)) {
            if (e.is_directory(ec)) {
                addWatch(e.path().string());
            }
        }
    }

    struct pollfd pfds[2];
    pfds[0] = {fd, POLLIN, 0};
    pfds[1] = {g_stopPipe[0], POLLIN, 0};
    alignas(struct inotify_event) char buf[8192];

    while (!g_watchStop.load()) {
        if (poll(pfds, 2, -1) < 0) {
            if (errno == EINTR) {
                continue;
            }
            break;
        }
        if (pfds[1].revents & POLLIN) {
            break;  // stop requested
        }
        if (!(pfds[0].revents & POLLIN)) {
            continue;
        }
        const ssize_t len = read(fd, buf, sizeof(buf));
        for (char* p = buf; len > 0 && p < buf + len;) {
            auto* ev = reinterpret_cast<struct inotify_event*>(p);
            auto it = watched.find(ev->wd);
            if (it != watched.end() && ev->len > 0 && (ev->mask & IN_ISDIR) &&
                (ev->mask & IN_CREATE)) {
                addWatch(it->second + "/" + ev->name);  // a mod folder (re)appeared
            }
            // React once a write has finished (IN_CLOSE_WRITE) or a file was moved
            // into place (IN_MOVED_TO) -- never on IN_CREATE, which fires mid-write
            // and would load a half-written library.
            if (!(ev->mask & IN_ISDIR) && (ev->mask & (IN_CLOSE_WRITE | IN_MOVED_TO))) {
                g_libsChanged.store(true);
            }
            p += sizeof(struct inotify_event) + ev->len;
        }
    }

    for (const auto& kv : watched) {
        inotify_rm_watch(fd, kv.first);
    }
    close(fd);
}

static void startWatcher(const std::string& root) {
    if (pipe(g_stopPipe) != 0) {
        return;
    }
    g_watchStop.store(false);
    g_watchThread = std::thread(watch_thread, root);
}

static void stopWatcher() {
    if (!g_watchThread.joinable()) {
        return;
    }
    g_watchStop.store(true);
    const char c = 'x';
    [[maybe_unused]] ssize_t w = write(g_stopPipe[1], &c, 1);
    g_watchThread.join();
    close(g_stopPipe[0]);
    close(g_stopPipe[1]);
    g_stopPipe[0] = g_stopPipe[1] = -1;
}

#elif defined(_WIN32)
static HANDLE g_stopEvent = nullptr;

static void watch_thread(std::string root) {
    const HANDLE change = FindFirstChangeNotificationA(
        root.c_str(), TRUE, FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_FILE_NAME);
    if (change == INVALID_HANDLE_VALUE) {
        return;
    }
    HANDLE handles[2] = {change, g_stopEvent};
    while (!g_watchStop.load()) {
        const DWORD r = WaitForMultipleObjects(2, handles, FALSE, INFINITE);
        if (r != WAIT_OBJECT_0) {
            break;  // stop event or error
        }
        g_libsChanged.store(true);
        FindNextChangeNotification(change);
    }
    FindCloseChangeNotification(change);
}

static void startWatcher(const std::string& root) {
    g_stopEvent = CreateEventA(nullptr, TRUE, FALSE, nullptr);
    g_watchStop.store(false);
    g_watchThread = std::thread(watch_thread, root);
}

static void stopWatcher() {
    if (!g_watchThread.joinable()) {
        return;
    }
    g_watchStop.store(true);
    if (g_stopEvent) {
        SetEvent(g_stopEvent);
    }
    g_watchThread.join();
    if (g_stopEvent) {
        CloseHandle(g_stopEvent);
        g_stopEvent = nullptr;
    }
}

#elif defined(__APPLE__)
// kqueue watcher (works on macOS + iOS; plain BSD, no CoreServices/Carbon).
static int g_kq = -1;
static std::vector<int> g_watchedFds;
static constexpr uintptr_t kStopIdent = 1;

static void watch_thread(std::string root) {
    g_kq = kqueue();
    if (g_kq < 0) {
        return;
    }
    // A user event we can trigger from stopWatcher() to wake the kevent() wait.
    struct kevent stopEv;
    EV_SET(&stopEv, kStopIdent, EVFILT_USER, EV_ADD | EV_CLEAR, 0, 0, nullptr);
    kevent(g_kq, &stopEv, 1, nullptr, 0, nullptr);

    const auto addWatch = [&](const std::string& path) {
        const int fd = open(path.c_str(), O_EVTONLY);
        if (fd < 0) {
            return;
        }
        struct kevent kev;
        EV_SET(&kev, fd, EVFILT_VNODE, EV_ADD | EV_CLEAR,
            NOTE_WRITE | NOTE_EXTEND | NOTE_DELETE | NOTE_RENAME | NOTE_ATTRIB, 0, nullptr);
        if (kevent(g_kq, &kev, 1, nullptr, 0, nullptr) == 0) {
            g_watchedFds.push_back(fd);
        } else {
            close(fd);
        }
    };

    // Watch the mods root, each mod entry, and the files within each mod folder.
    std::error_code ec;
    if (fs::exists(root, ec)) {
        addWatch(root);
        for (const auto& e : fs::directory_iterator(root, ec)) {
            addWatch(e.path().string());
            if (e.is_directory(ec)) {
                for (const auto& f : fs::directory_iterator(e.path(), ec)) {
                    addWatch(f.path().string());
                }
            }
        }
    }

    while (!g_watchStop.load()) {
        struct kevent out;
        const int n = kevent(g_kq, nullptr, 0, &out, 1, nullptr);
        if (n < 0) {
            if (errno == EINTR) {
                continue;
            }
            break;
        }
        if (n == 0) {
            continue;
        }
        if (out.filter == EVFILT_USER) {
            break;  // stop requested
        }
        g_libsChanged.store(true);
    }

    for (const int fd : g_watchedFds) {
        close(fd);
    }
    g_watchedFds.clear();
    close(g_kq);
    g_kq = -1;
}

static void startWatcher(const std::string& root) {
    g_watchStop.store(false);
    g_watchThread = std::thread(watch_thread, root);
}

static void stopWatcher() {
    if (!g_watchThread.joinable()) {
        return;
    }
    g_watchStop.store(true);
    if (g_kq >= 0) {
        struct kevent kev;
        EV_SET(&kev, kStopIdent, EVFILT_USER, 0, NOTE_TRIGGER, 0, nullptr);
        kevent(g_kq, &kev, 1, nullptr, 0, nullptr);
    }
    g_watchThread.join();
}

#else
// No file watcher on other platforms: mods still load and can be enabled/disabled
// + refreshed; rebuilt libraries just need a Refresh/restart.
static void startWatcher(const std::string&) {}
static void stopWatcher() {}
#endif

namespace dusk {

ModLoader& ModLoader::instance() {
    return g_modLoader;
}

// ---- bundle + native loading (unchanged core) ------------------------------

static std::unique_ptr<ModBundle> loadBundle(const fs::path& modPath, bool fromDir) {
    if (fromDir) {
        return std::make_unique<ModBundleDisk>(modPath);
    }
    std::vector<u8> data = io::FileStream::ReadAllBytes(modPath);
    return std::make_unique<ModBundleZip>(std::move(data));
}

struct DllLocateResult {
    std::string primary;
    std::string fallback;
};

static std::string_view getFileNameWithoutExtension(const std::string_view fileName) {
    return fileName.substr(0, fileName.find_last_of('.'));
}

static DllLocateResult LocateDllInBundle(ModBundle& bundle) {
    std::string dllEntry, dllFallback;
    for (const auto& name : bundle.getFileNames()) {
        if (!name.ends_with(".dll"sv) && !name.ends_with(".dylib"sv) && !name.ends_with(".so"sv)) {
            continue;
        }

        if (!k_archSuffix.empty() && getFileNameWithoutExtension(name).ends_with(k_archSuffix)) {
            dllEntry = name;
        } else if (dllFallback.empty()) {
            dllFallback = name;
        }
    }

    return DllLocateResult{dllEntry, dllFallback};
}

class InvalidModDataException : public std::runtime_error {
public:
    explicit InvalidModDataException(const std::string& msg) : runtime_error(msg) {}
    explicit InvalidModDataException(const char* msg) : runtime_error(msg) {}
};

static void validateModId(std::string_view const str) {
    if (str.empty()) {
        throw InvalidModDataException("Missing ID value in mod metadata!");
    }

    bool lastWasPeriod = false;
    for (auto const chr : str) {
        if (chr == '.') {
            if (lastWasPeriod) {
                throw InvalidModDataException("Cannot have two consecutive periods in mod ID!");
            }
            lastWasPeriod = true;
            continue;
        }
        lastWasPeriod = false;

        if (chr == '_' || (chr >= '0' && chr <= '9') || (chr >= 'a' && chr <= 'z') ||
            (chr >= 'A' && chr <= 'Z')) {
            continue;
        }

        throw InvalidModDataException(fmt::format(
            "Invalid character '{}' in mod ID. Valid characters are period, underscore, and alphanumerics.",
            chr));
    }
}

static ModMetadata loadMetadata(const fs::path& modPath, ModBundle& bundle) {
    const auto metaJson = bundle.readFile("mod.json");
    auto j = json::parse(metaJson);

    std::string metaId = j.value("id", "");
    std::string metaName = j.value("name", "");
    std::string metaVersion = j.value("version", "");
    std::string metaAuthor = j.value("author", "");
    // Accept "description" (this branch) or "about" (our older mods) interchangeably.
    std::string metaDescription = j.value("description", j.value("about", ""s));
    const bool hasCode = j.value("has_code", false);
    const int priority = j.value("priority", 0);

    std::vector<std::string> deps;
    if (auto it = j.find("dependencies"); it != j.end() && it->is_array()) {
        for (const auto& d : *it) {
            if (d.is_string()) {
                deps.push_back(d.get<std::string>());
            }
        }
    }

    validateModId(metaId);

    if (metaName.empty()) {
        metaName = io::fs_path_to_string(modPath.stem());
    }
    if (metaVersion.empty()) {
        metaVersion = "?"s;
    }
    if (metaAuthor.empty()) {
        metaAuthor = "unknown"s;
    }

    return ModMetadata{
        std::move(metaId), std::move(metaName), std::move(metaVersion), std::move(metaAuthor),
        std::move(metaDescription), hasCode, priority, std::move(deps),
    };
}

template <std::ranges::input_range TIter>
static bool checkDuplicateMod(const ModMetadata& metadata, TIter mods) {
    return std::ranges::any_of(
        mods, [&](const LoadedMod& mod) { return mod.metadata.id == metadata.id; });
}

void ModLoader::tryLoadNativeMod(LoadedMod& mod) {
    if (!EnableCodeMods) {
        Log.error("Code mods are not available in this build");
        mod.native_status = NativeModStatus::BuildDisabled;
        return;
    }

    auto [dllEntry, dllFallback] = LocateDllInBundle(*mod.bundle);
    if (dllEntry.empty()) {
        dllEntry = dllFallback;
    }

    if (dllEntry.empty()) {
        Log.error("no *{} found in {} — skipping", NativeModule::LibraryExtension, mod.metadata.id);
        mod.native_status = NativeModStatus::ModMissingPlatform;
        return;
    }

    std::error_code ec;
    // Record the on-disk source file the watcher follows *before* attempting the
    // load, so even a failed load is hot-reload-retried when the file is rebuilt.
    mod.source_lib =
        mod.fromDir ? io::fs_path_to_string(fs::path(mod.mod_path) / dllEntry) : mod.mod_path;
    mod.lib_mtime = fs::last_write_time(mod.source_lib, ec);

    // Fresh cache dir each load so hot-reload always maps a new inode (avoids
    // dlopen returning the previously-loaded image).
    const fs::path cacheDir = m_modsDir / ".cache" / mod.metadata.id;
    fs::remove_all(cacheDir, ec);
    fs::create_directories(cacheDir, ec);

    const fs::path dllCachePath = cacheDir / fs::path(dllEntry).filename();

    std::vector<u8> dllData;
    try {
        dllData = mod.bundle->readFile(dllEntry);
    } catch (const std::runtime_error& e) {
        Log.error("failed to extract {} from {}", dllEntry, mod.metadata.id);
        return;
    }

    {
        std::ofstream out(dllCachePath, std::ios::binary | std::ios::out);
        if (!out) {
            Log.error("failed to write {}", io::fs_path_to_string(dllCachePath));
            return;
        }
        out.write(reinterpret_cast<const char*>(dllData.data()),
            static_cast<std::streamsize>(dllData.size()));
    }

    auto nativeMod = std::make_unique<NativeMod>();
    try {
        nativeMod->handle = std::make_unique<NativeModule>(dllCachePath);
    } catch (const std::runtime_error& e) {
        Log.error("failed to open {}: {}", io::fs_path_to_string(dllCachePath), e.what());
        return;
    }

    const auto mod_api_ver = nativeMod->handle->LookupSymbol<uint32_t*>("mod_api_version");
    if (mod_api_ver && *mod_api_ver != DUSK_MOD_API_VERSION) {
        Log.error("{} expects API v{} but engine is v{}, skipping",
            io::fs_path_to_string(fs::path(dllEntry).filename()), *mod_api_ver, DUSK_MOD_API_VERSION);
        mod.native_status = NativeModStatus::ApiVersionMismatch;
        return;
    }

    nativeMod->fn_init = nativeMod->handle->LookupSymbol<NativeMod::FnInit>("mod_init");
    nativeMod->fn_tick = nativeMod->handle->LookupSymbol<NativeMod::FnTick>("mod_tick");
    nativeMod->fn_cleanup = nativeMod->handle->LookupSymbol<NativeMod::FnCleanup>("mod_cleanup");

    if (!nativeMod->fn_init || !nativeMod->fn_tick) {
        Log.error("{} missing mod_init or mod_tick — skipping",
            io::fs_path_to_string(fs::path(dllEntry).filename()));
        return;
    }

    mod.dir = io::fs_path_to_string(fs::absolute(cacheDir));
    mod.native = std::move(nativeMod);
    mod.native_status = NativeModStatus::Loaded;
    mod.lib_mtime = fs::last_write_time(mod.source_lib, ec);  // refresh to the loaded mtime
}

void ModLoader::tryLoadDusk(const fs::path& modPath, bool fromDir) {
    std::unique_ptr<ModBundle> bundle;
    try {
        bundle = loadBundle(modPath, fromDir);
    } catch (const std::runtime_error& e) {
        Log.error(
            "Failed to open {} bundle: {}", io::fs_path_to_string(modPath.filename()), e.what());
        return;
    }

    ModMetadata metadata;
    try {
        metadata = loadMetadata(modPath, *bundle);
    } catch (const std::runtime_error& e) {
        Log.error("bad mod.json in {}: {}", io::fs_path_to_string(modPath.filename()), e.what());
        return;
    }

    if (checkDuplicateMod(metadata, mods())) {
        Log.error("mod with id '{}' already exists, not loading {}", metadata.id,
            io::fs_path_to_string(modPath.filename()));
        return;
    }

    auto& mod = *m_mods.emplace_back(std::make_unique<LoadedMod>());
    mod.fromDir = fromDir;
    mod.mod_path = io::fs_path_to_string(fs::absolute(modPath));
    mod.metadata = std::move(metadata);
    mod.bundle = std::move(bundle);
    mod.cvarIsEnabled =
        std::make_unique<ConfigVar<bool>>(modEnabledCVarName(mod.metadata.id), true);

    if (mod.metadata.hasCode) {
        mod.native_status = NativeModStatus::Unknown;
        tryLoadNativeMod(mod);
        // A native load failure does not block insertion -- we still present the
        // failed mod in the UI.
        if (mod.native_status != NativeModStatus::Loaded) {
            Log.error("Native mod '{}' failed to load", mod.metadata.id);
        }
    }

    Log.info("found '{}' ('{}') v{} by {} ({})", mod.metadata.name, mod.metadata.id,
        mod.metadata.version, mod.metadata.author, io::fs_path_to_string(modPath.filename()));
}

// ---- per-mod config.json (enabled + values) + settings schema --------------

static ModSetting* findSetting(LoadedMod& mod, std::string_view key) {
    for (auto& s : mod.settings) {
        if (s.key == key) {
            return &s;
        }
    }
    return nullptr;
}

static const char* settingTypeName(SettingType type) {
    switch (type) {
    case SettingType::Bool: return "bool";
    case SettingType::Int: return "int";
    default: return "float";
    }
}

static SettingType settingTypeFromName(std::string_view name) {
    if (name == "bool") return SettingType::Bool;
    if (name == "int") return SettingType::Int;
    return SettingType::Float;
}

fs::path ModLoader::configPath(const LoadedMod& mod) const {
    return m_modsDir / ".config" / (mod.metadata.id + ".json");
}

fs::path ModLoader::schemaPath(const LoadedMod& mod) const {
    return m_modsDir / ".config" / (mod.metadata.id + ".schema.json");
}

void ModLoader::readConfig(LoadedMod& mod) {
    const fs::path path = configPath(mod);
    std::error_code ec;
    if (!fs::exists(path, ec)) {
        return;  // keep defaults
    }
    try {
        const json j = json::parse(io::FileStream::ReadAllBytes(path));
        if (!j.is_object()) {
            return;
        }
        // config.json holds only the mod's setting values; enabled is a CVar.
        if (auto settings = j.find("settings"); settings != j.end() && settings->is_object()) {
            for (ModSetting& s : mod.settings) {
                if (auto v = settings->find(s.key); v != settings->end() && v->is_number()) {
                    s.value = v->get<double>();
                }
            }
        }
    } catch (const std::exception& e) {
        Log.warn("mod '{}': failed to read config.json: {}", mod.metadata.id, e.what());
    }
}

void ModLoader::writeConfig(const LoadedMod& mod) {
    json settings = json::object();
    for (const ModSetting& s : mod.settings) {
        settings[s.key] = s.value;
    }
    const json j = json{{"settings", settings}};  // enabled is a game-owned CVar
    std::error_code ec;
    fs::create_directories(configPath(mod).parent_path(), ec);
    try {
        io::FileStream::WriteAllText(configPath(mod), j.dump(4));
    } catch (const std::exception& e) {
        Log.warn("mod '{}': failed to write config.json: {}", mod.metadata.id, e.what());
    }
}

void ModLoader::writeSchema(const LoadedMod& mod) {
    json arr = json::array();
    for (const ModSetting& s : mod.settings) {
        arr.push_back(json{
            {"key", s.key},
            {"label", s.label},
            {"help", s.help},
            {"type", settingTypeName(s.type)},
            {"default", s.defaultValue},
            {"min", s.minValue},
            {"max", s.maxValue},
            {"step", s.step},
        });
    }
    std::error_code ec;
    fs::create_directories(schemaPath(mod).parent_path(), ec);
    try {
        io::FileStream::WriteAllText(schemaPath(mod), arr.dump(4));
    } catch (const std::exception& e) {
        Log.warn("mod '{}': failed to write settings schema: {}", mod.metadata.id, e.what());
    }
}

// Load a mod's declared settings from a schema written on a prior run, so the UI
// can show controls for a mod that isn't currently loaded.
void ModLoader::parseSchema(LoadedMod& mod) {
    const fs::path path = schemaPath(mod);
    std::error_code ec;
    if (!fs::exists(path, ec)) {
        return;
    }
    try {
        const json arr = json::parse(io::FileStream::ReadAllBytes(path));
        if (!arr.is_array()) {
            return;
        }
        std::vector<ModSetting> parsed;
        for (const auto& e : arr) {
            if (!e.is_object() || !e.contains("key")) {
                continue;
            }
            ModSetting s;
            s.key = e.value("key", "");
            s.label = e.value("label", s.key);
            s.help = e.value("help", "");
            s.type = settingTypeFromName(e.value("type", "float"));
            s.defaultValue = e.value("default", 0.0);
            s.minValue = e.value("min", 0.0);
            s.maxValue = e.value("max", 0.0);
            s.step = e.value("step", 0.0);
            s.value = s.defaultValue;
            parsed.push_back(std::move(s));
        }
        mod.settings = std::move(parsed);
    } catch (const std::exception& e) {
        Log.warn("mod '{}': failed to read settings schema: {}", mod.metadata.id, e.what());
    }
}

void ModLoader::applySetting(LoadedMod& mod, std::string_view key, double value) {
    ModSetting* s = findSetting(mod, key);
    if (s == nullptr || s->value == value) {
        return;
    }
    s->value = value;
    if (g_loadingMod) {
        return;  // don't persist while the mod is still initializing
    }
    writeConfig(mod);
}

// ---- mod API surface (operate on the calling mod) --------------------------

void ModLoader::modDefineSettings(LoadedMod& mod, const DuskSetting* arr, uint32_t count) {
    std::vector<ModSetting> rebuilt;
    rebuilt.reserve(count);
    for (uint32_t i = 0; i < count; ++i) {
        const DuskSetting& src = arr[i];
        if (src.key == nullptr) {
            continue;
        }
        ModSetting s;
        s.key = src.key;
        s.label = src.label ? src.label : src.key;
        s.help = src.help ? src.help : "";
        s.type = static_cast<SettingType>(src.type);
        s.defaultValue = src.default_value;
        s.minValue = src.min_value;
        s.maxValue = src.max_value;
        s.step = src.step;
        s.value = src.default_value;
        // Preserve any live value for this key so a reload never resets settings.
        if (const ModSetting* prev = findSetting(mod, s.key)) {
            s.value = prev->value;
        }
        rebuilt.push_back(std::move(s));
    }
    mod.settings = std::move(rebuilt);
    readConfig(mod);   // overlay saved setting values
    writeSchema(mod);  // (re)generate the schema for the UI
}

double ModLoader::modGetSetting(LoadedMod& mod, const char* key) {
    ModSetting* s = findSetting(mod, key ? key : "");
    return s ? s->value : 0.0;
}

void ModLoader::modSetSetting(LoadedMod& mod, const char* key, double value) {
    applySetting(mod, key ? key : "", value);
}

// ---- load / unload / reload lifecycle --------------------------------------

bool ModLoader::initMod(LoadedMod& mod) {
    if (!mod.native) {
        return false;
    }
    buildAPI(mod);

    ModGuard guard(&mod);
    g_loadingMod = true;
    mod.load_failed = false;
    bool ok = false;
    try {
        mod.native->fn_init(&mod.native->api);
        ok = !mod.load_failed;
        if (ok) {
            mod.load_error.clear();
            Log.info("'{}' initialized", mod.metadata.id);
        } else {
            Log.error("'{}' failed to load due to hook conflicts", mod.metadata.id);
        }
    } catch (const std::exception& e) {
        Log.error("exception in {}.mod_init(): {}", mod.metadata.id, e.what());
    } catch (...) {
        Log.error("unknown exception in {}.mod_init()", mod.metadata.id);
    }
    g_loadingMod = false;
    mod.active = ok;
    return ok;
}

void ModLoader::unloadMod(LoadedMod& mod) {
    hookClearMod(&mod);
    if (mod.native && mod.native->fn_cleanup) {
        ModGuard guard(&mod);
        try {
            mod.native->fn_cleanup(&mod.native->api);
        } catch (...) {
        }
    }
    mod.tab_content.clear();
    mod.tab_updates.clear();
    mod.native.reset();  // dlclose
    mod.active = false;
    mod.load_failed = false;
}

void ModLoader::reloadMod(LoadedMod& mod) {
    unloadMod(mod);
    if (!mod.enabled) {
        return;
    }
    try {
        mod.bundle = loadBundle(mod.mod_path, mod.fromDir);
    } catch (const std::runtime_error& e) {
        Log.error("hot-reload '{}': can't reopen bundle: {}", mod.metadata.id, e.what());
        modToast("Mod reload failed", mod.metadata.name, 6);
        return;
    }
    mod.native_status = NativeModStatus::Unknown;
    tryLoadNativeMod(mod);
    if (mod.native_status != NativeModStatus::Loaded) {
        Log.error("hot-reload '{}': native load failed", mod.metadata.id);
        modToast("Mod reload failed", mod.metadata.name, 6);
        return;
    }
    const bool ok = initMod(mod);
    Log.info("hot-reloaded '{}'", mod.metadata.id);
    if (ok) {
        modToast("Mod reloaded", mod.metadata.name, 3);
    } else {
        modToast("Mod reload failed", mod.metadata.name + " (init error)", 6);
    }
}

// ---- public lifecycle ------------------------------------------------------

// Reorder m_mods so every mod comes after its dependencies, breaking ties by
// priority (lower first) then id. Kahn's algorithm; cyclic mods are marked failed
// and appended. Only the unique_ptrs move -- LoadedMod addresses stay stable.
void ModLoader::computeLoadOrder() {
    const std::size_t n = m_mods.size();
    if (n < 2) {
        return;
    }
    std::unordered_map<std::string, std::size_t> index;
    for (std::size_t i = 0; i < n; ++i) {
        index[m_mods[i]->metadata.id] = i;
    }
    std::vector<int> indeg(n, 0);
    std::vector<std::vector<std::size_t>> dependents(n);
    for (std::size_t i = 0; i < n; ++i) {
        for (const std::string& dep : m_mods[i]->metadata.dependencies) {
            if (auto it = index.find(dep); it != index.end()) {
                dependents[it->second].push_back(i);
                indeg[i]++;
            }
        }
    }
    const auto better = [&](std::size_t a, std::size_t b) {
        if (m_mods[a]->metadata.priority != m_mods[b]->metadata.priority) {
            return m_mods[a]->metadata.priority < m_mods[b]->metadata.priority;
        }
        return m_mods[a]->metadata.id < m_mods[b]->metadata.id;
    };
    std::vector<std::size_t> ready;
    for (std::size_t i = 0; i < n; ++i) {
        if (indeg[i] == 0) {
            ready.push_back(i);
        }
    }
    std::vector<std::size_t> order;
    order.reserve(n);
    while (!ready.empty()) {
        auto best = std::min_element(ready.begin(), ready.end(), better);
        const std::size_t i = *best;
        ready.erase(best);
        order.push_back(i);
        for (const std::size_t j : dependents[i]) {
            if (--indeg[j] == 0) {
                ready.push_back(j);
            }
        }
    }
    if (order.size() < n) {  // leftovers are in a dependency cycle
        std::vector<bool> placed(n, false);
        for (const std::size_t i : order) {
            placed[i] = true;
        }
        std::vector<std::size_t> leftover;
        for (std::size_t i = 0; i < n; ++i) {
            if (!placed[i]) {
                leftover.push_back(i);
            }
        }
        std::sort(leftover.begin(), leftover.end(), better);
        for (const std::size_t i : leftover) {
            m_mods[i]->load_failed = true;
            m_mods[i]->load_error = "dependency cycle";
            order.push_back(i);
        }
    }
    std::vector<std::unique_ptr<LoadedMod>> reordered;
    reordered.reserve(n);
    for (const std::size_t i : order) {
        reordered.push_back(std::move(m_mods[i]));
    }
    m_mods = std::move(reordered);
}

bool ModLoader::depsSatisfied(const LoadedMod& mod) {
    for (const std::string& dep : mod.metadata.dependencies) {
        const LoadedMod* d = find(dep);
        if (d == nullptr || !d->active) {
            return false;
        }
    }
    return true;
}

bool ModLoader::checkDependencies(LoadedMod& mod) {
    for (const std::string& dep : mod.metadata.dependencies) {
        const LoadedMod* d = find(dep);
        if (d == nullptr) {
            mod.load_failed = true;
            mod.load_error = fmt::format("missing dependency '{}'", dep);
            return false;
        }
        if (!d->active) {
            mod.load_failed = true;
            mod.load_error = fmt::format("dependency '{}' not enabled", dep);
            return false;
        }
    }
    return true;
}

void ModLoader::init() {
    if (m_initialized) {
        return;
    }
    m_initialized = true;

    if (!fs::is_directory(m_modsDir)) {
        Log.info("mods directory '{}' not found — mod loading skipped",
            io::fs_path_to_string(m_modsDir));
        return;
    }

    std::error_code ec;
    std::vector<fs::directory_entry> entries;
    for (auto& e : fs::directory_iterator(m_modsDir, ec)) {
        if (e.is_directory() && fs::exists(e.path() / "mod.json")) {
            entries.push_back(e);
        } else if (e.is_regular_file() && e.path().extension() == ".dusk") {
            entries.push_back(e);
        }
    }
    std::sort(entries.begin(), entries.end(),
        [](const fs::directory_entry& a, const fs::directory_entry& b) {
            return a.path().filename() < b.path().filename();
        });

    m_mods.reserve(entries.size());
    for (auto& entry : entries) {
        tryLoadDusk(entry.path(), entry.is_directory());
    }

    computeLoadOrder();  // dependencies first, then priority

    // Always start the watcher so newly-added mods can be picked up via Refresh
    // and rebuilt libraries hot-reload.
    startWatcher(io::fs_path_to_string(m_modsDir));

    if (m_mods.empty()) {
        Log.info("no mods found");
        return;
    }

    // Enabled state is a game-owned CVar; settings descriptors + saved values
    // come from the per-mod config.
    for (auto& mod : mods()) {
        config::Register(*mod.cvarIsEnabled);
        mod.enabled = mod.cvarIsEnabled->getValue();
        parseSchema(mod);
        readConfig(mod);
    }

    Log.info("initializing {} mod(s)...", m_mods.size());
    for (auto& mod : mods()) {
        if (!mod.enabled) {
            Log.info("Mod '{}' is disabled by config", mod.metadata.id);
            continue;
        }
        if (mod.load_failed) {
            continue;  // already failed (e.g. dependency cycle)
        }
        if (!checkDependencies(mod)) {  // deps load earlier in order, so this is final
            Log.error("Mod '{}': {}", mod.metadata.id, mod.load_error);
            continue;
        }
        if (mod.metadata.hasCode) {
            if (mod.native) {
                initMod(mod);
            }
        } else {
            mod.active = true;  // non-code (overlay) mod
        }
    }

    initOverlayFiles();

    auto active = std::ranges::count_if(mods(), [](const LoadedMod& m) { return m.active; });
    Log.info("{}/{} mod(s) active", active, m_mods.size());
}

void ModLoader::tick() {
    update();  // hot-reload check (cheap unless the watcher saw a change)

    for (auto& mod : active_mods()) {
        if (!mod.native) {
            continue;
        }
        ModGuard guard(&mod);
        try {
            mod.native->fn_tick(&mod.native->api);
        } catch (const std::exception& e) {
            Log.error("exception in {}.mod_tick(): {} — disabling", mod.metadata.id, e.what());
            mod.active = false;
            modToast("Mod crashed (disabled)", mod.metadata.name, 6);
        } catch (...) {
            Log.error("unknown exception in {}.mod_tick() — disabling", mod.metadata.id);
            mod.active = false;
            modToast("Mod crashed (disabled)", mod.metadata.name, 6);
        }
    }
}

void ModLoader::update() {
    if (!g_libsChanged.exchange(false)) {
        return;
    }
    for (auto& mod : mods()) {
        if (!mod.enabled || !mod.metadata.hasCode || mod.source_lib.empty()) {
            continue;
        }
        std::error_code ec;
        const fs::file_time_type t = fs::last_write_time(mod.source_lib, ec);
        if (ec || t == mod.lib_mtime) {
            continue;
        }
        Log.info("mod '{}' library changed; hot-reloading", mod.metadata.id);
        reloadMod(mod);
    }
}

void ModLoader::refresh() {
    if (!fs::is_directory(m_modsDir)) {
        return;
    }
    std::error_code ec;
    const std::size_t before = m_mods.size();
    std::vector<fs::directory_entry> entries;
    for (auto& e : fs::directory_iterator(m_modsDir, ec)) {
        if (e.is_directory() && fs::exists(e.path() / "mod.json")) {
            entries.push_back(e);
        } else if (e.is_regular_file() && e.path().extension() == ".dusk") {
            entries.push_back(e);
        }
    }
    std::sort(entries.begin(), entries.end(),
        [](const fs::directory_entry& a, const fs::directory_entry& b) {
            return a.path().filename() < b.path().filename();
        });

    for (auto& entry : entries) {
        tryLoadDusk(entry.path(), entry.is_directory());  // skips ids already loaded
    }

    bool addedOverlay = false;
    for (std::size_t i = before; i < m_mods.size(); ++i) {
        LoadedMod& mod = *m_mods[i];
        config::Register(*mod.cvarIsEnabled);
        mod.enabled = mod.cvarIsEnabled->getValue();
        parseSchema(mod);
        readConfig(mod);
        if (!mod.enabled) {
            continue;
        }
        if (!checkDependencies(mod)) {
            Log.error("Mod '{}': {}", mod.metadata.id, mod.load_error);
            continue;
        }
        if (mod.metadata.hasCode) {
            if (mod.native) {
                initMod(mod);
            }
        } else {
            mod.active = true;
            addedOverlay = true;
        }
    }
    if (addedOverlay) {
        initOverlayFiles();
    }
    Log.info("refresh: {} new mod(s)", m_mods.size() - before);
}

void ModLoader::shutdown() {
    stopWatcher();
    for (auto& mod : mods()) {
        unloadMod(mod);
        OrphanedConfigVars.emplace_back(std::move(mod.cvarIsEnabled));
    }
    m_mods.clear();
    g_services.clear();
    Log.info("all mods unloaded");
}

// ---- lookup + live enable/disable + settings (by id, for the UI) -----------

LoadedMod* ModLoader::find(std::string_view id) {
    for (auto& m : m_mods) {
        if (m->metadata.id == id) {
            return m.get();
        }
    }
    return nullptr;
}

bool ModLoader::isEnabled(std::string_view id) {
    const LoadedMod* m = find(id);
    return m != nullptr && m->enabled;
}

void ModLoader::setEnabled(std::string_view id, bool enabled) {
    LoadedMod* m = find(id);
    if (m == nullptr || m->enabled == enabled) {
        return;
    }
    if (enabled && !checkDependencies(*m)) {
        // Can't enable until dependencies are installed + enabled. Leave it off.
        modToast("Can't enable mod", m->metadata.name + ": " + m->load_error, 6);
        return;
    }
    m->enabled = enabled;
    if (m->cvarIsEnabled) {
        m->cvarIsEnabled->setValue(enabled);  // game-owned, persisted by config::Save
    }

    if (enabled) {
        if (m->metadata.hasCode) {
            if (!m->native) {
                try {
                    m->bundle = loadBundle(m->mod_path, m->fromDir);
                    m->native_status = NativeModStatus::Unknown;
                    tryLoadNativeMod(*m);
                } catch (const std::runtime_error& e) {
                    Log.error("enable '{}': can't open bundle: {}", m->metadata.id, e.what());
                }
            }
            if (m->native && !initMod(*m)) {
                modToast("Mod failed to enable", m->metadata.name, 6);
            }
        } else {
            m->active = true;
            initOverlayFiles();
        }
    } else {
        unloadMod(*m);
    }
    config::Save();
}

double ModLoader::getSetting(std::string_view id, std::string_view key) {
    LoadedMod* m = find(id);
    if (m == nullptr) {
        return 0.0;
    }
    ModSetting* s = findSetting(*m, key);
    return s ? s->value : 0.0;
}

void ModLoader::setSetting(std::string_view id, std::string_view key, double value) {
    if (LoadedMod* m = find(id)) {
        applySetting(*m, key, value);
    }
}

}  // namespace dusk
