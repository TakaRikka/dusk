#include "loader.hpp"
#include "dusk/logging.h"
#include "dusk/mod_loader.hpp"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <utility>

#include "../manifest.hpp"
#include "depgraph.hpp"
#include "dusk/config.hpp"
#include "dusk/io.hpp"
#include "dusk/mods/log_buffer.hpp"
#include "dusk/mods/svc/config.hpp"
#include "dusk/mods/svc/registry.hpp"
#include "dusk/ui/mods_window.hpp"
#include "dusk/ui/ui.hpp"
#include "miniz.h"
#include "native_module.hpp"
#include "nlohmann/json.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

#if defined(_WIN32)
#if defined(_M_ARM64)
static constexpr std::string_view k_nativeLibName = "windows-arm64.dll"sv;
#elif defined(_M_X64)
static constexpr std::string_view k_nativeLibName = "windows-amd64.dll"sv;
#elif defined(_M_IX86)
static constexpr std::string_view k_nativeLibName = "windows-x86.dll"sv;
#else
static constexpr std::string_view k_nativeLibName = ""sv;
#endif
#elif defined(__ANDROID__)
#if defined(__aarch64__)
static constexpr std::string_view k_nativeLibName = "android-aarch64.so"sv;
#elif defined(__x86_64__)
static constexpr std::string_view k_nativeLibName = "android-x86_64.so"sv;
#else
static constexpr std::string_view k_nativeLibName = ""sv;
#endif
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#if TARGET_OS_IOS
static constexpr std::string_view k_nativeLibName = "ios-arm64.dylib"sv;
#elif TARGET_OS_TV
static constexpr std::string_view k_nativeLibName = "tvos-arm64.dylib"sv;
#elif defined(__aarch64__)
static constexpr std::string_view k_nativeLibName = "darwin-arm64.dylib"sv;
#elif defined(__x86_64__)
static constexpr std::string_view k_nativeLibName = "darwin-x86_64.dylib"sv;
#else
static constexpr std::string_view k_nativeLibName = ""sv;
#endif
#elif defined(__linux__)
#if defined(__aarch64__)
static constexpr std::string_view k_nativeLibName = "linux-aarch64.so"sv;
#elif defined(__x86_64__)
static constexpr std::string_view k_nativeLibName = "linux-x86_64.so"sv;
#elif defined(__i386__)
static constexpr std::string_view k_nativeLibName = "linux-x86.so"sv;
#else
static constexpr std::string_view k_nativeLibName = ""sv;
#endif
#else
static constexpr std::string_view k_nativeLibName = ""sv;
#endif

namespace dusk::mods {
namespace {
aurora::Module Log{"dusk::mods::loader"};
ModLoader g_modLoader;

std::unique_ptr<ModBundle> load_bundle(const std::filesystem::path& modPath, bool fromDir) {
    if (fromDir) {
        return std::make_unique<ModBundleDisk>(modPath);
    } else {
        std::vector<u8> data = io::FileStream::ReadAllBytes(modPath);
        return std::make_unique<ModBundleZip>(std::move(data));
    }
}

struct DllLocateResult {
    std::string entry;
    bool anyLibs = false;
};

DllLocateResult locate_dll_in_bundle(ModBundle& bundle) {
    DllLocateResult result;
    for (const auto& name : bundle.getFileNames()) {
        if (name.find('/') != std::string::npos ||
            (!name.ends_with(".dll"sv) && !name.ends_with(".dylib"sv) && !name.ends_with(".so"sv)))
        {
            continue;
        }
        result.anyLibs = true;
        if (name == k_nativeLibName) {
            result.entry = name;
        }
    }
    return result;
}
}  // namespace

ModLoader& ModLoader::instance() {
    return g_modLoader;
}

class InvalidModDataException : public std::runtime_error {
public:
    explicit InvalidModDataException(const std::string& msg) : runtime_error(msg) {}
    explicit InvalidModDataException(const char* msg) : runtime_error(msg) {}
};

static void validate_mod_id(std::string_view const str) {
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

        if (chr == '_')
            continue;

        if (chr >= '0' && chr <= '9')
            continue;

        if (chr >= 'a' && chr <= 'z')
            continue;

        if (chr >= 'A' && chr <= 'Z')
            continue;

        throw InvalidModDataException(
            fmt::format("Invalid character '{}' in mod ID. Valid characters are period, "
                        "underscore, and alphanumerics.",
                chr));
    }
}

static bool bundle_has_file(ModBundle& bundle, const std::string& path) {
    try {
        bundle.getFileSize(path);
        return true;
    } catch (const std::runtime_error&) {
        return false;
    }
}

static std::string resolve_image_path(ModBundle& bundle, const std::string& modId,
    std::string_view key, const std::string& manifestPath, const std::string& defaultPath) {
    if (!manifestPath.empty()) {
        if (!is_safe_resource_path(manifestPath)) {
            log::write(
                modId, LOG_LEVEL_WARN, "invalid {} path '{}' in mod.json", key, manifestPath);
        } else if (!bundle_has_file(bundle, manifestPath)) {
            log::write(
                modId, LOG_LEVEL_WARN, "{} path '{}' not found in bundle", key, manifestPath);
        } else {
            return manifestPath;
        }
    }
    if (bundle_has_file(bundle, defaultPath)) {
        return defaultPath;
    }
    return {};
}

static ModMetadata load_metadata(const std::filesystem::path& modPath, ModBundle& bundle) {
    const auto metaJson = bundle.readFile("mod.json");
    auto j = nlohmann::json::parse(metaJson);

    std::string metaId = j.value("id", "");
    std::string metaName = j.value("name", "");
    std::string metaVersion = j.value("version", "");
    std::string metaAuthor = j.value("author", "");
    std::string metaDescription = j.value("description", "");
    std::string metaIcon = j.value("icon", "");
    std::string metaBanner = j.value("banner", "");

    validate_mod_id(metaId);

    if (metaName.empty()) {
        metaName = io::fs_path_to_string(modPath.stem());
    }
    if (metaVersion.empty()) {
        metaVersion = "?"s;
    }
    if (metaAuthor.empty()) {
        metaAuthor = "unknown"s;
    }

    std::string iconPath = resolve_image_path(bundle, metaId, "icon", metaIcon, "res/icon.png"s);
    std::string bannerPath =
        resolve_image_path(bundle, metaId, "banner", metaBanner, "res/banner.png"s);

    return ModMetadata{
        std::move(metaId),
        std::move(metaName),
        std::move(metaVersion),
        std::move(metaAuthor),
        std::move(metaDescription),
        std::move(iconPath),
        std::move(bannerPath),
    };
}

static bool validate_manifest(const ModManifest* manifest, LoadedMod& mod) {
    if (manifest == nullptr) {
        log::write(mod.metadata.id, LOG_LEVEL_ERROR, "returned a null mod manifest");
        mod.nativeStatus = NativeModStatus::MissingExport;
        return false;
    }
    if (manifest->struct_size != sizeof(ModManifest)) {
        log::write(mod.metadata.id, LOG_LEVEL_ERROR, "manifest has invalid size {} (expected {})",
            manifest->struct_size, sizeof(ModManifest));
        mod.nativeStatus = NativeModStatus::ApiVersionMismatch;
        return false;
    }
    if (manifest->abi_version != MOD_ABI_VERSION) {
        log::write(mod.metadata.id, LOG_LEVEL_ERROR, "expects ABI v{} but engine is v{}, skipping",
            manifest->abi_version, MOD_ABI_VERSION);
        mod.nativeStatus = NativeModStatus::ApiVersionMismatch;
        return false;
    }
    if ((manifest->import_count > 0 && manifest->imports == nullptr) ||
        (manifest->export_count > 0 && manifest->exports == nullptr))
    {
        log::write(mod.metadata.id, LOG_LEVEL_ERROR, "manifest has invalid import/export arrays");
        mod.nativeStatus = NativeModStatus::MissingExport;
        return false;
    }
    return true;
}

static bool validate_context_symbol(ModContext** contextSymbol, LoadedMod& mod) {
    if (contextSymbol == nullptr) {
        log::write(mod.metadata.id, LOG_LEVEL_ERROR, "missing required mod_ctx export");
        mod.nativeStatus = NativeModStatus::MissingExport;
        return false;
    }
    return true;
}

static std::string lifecycle_error_message(
    const char* fnName, const ModResult result, const ModError& error) {
    if (error.message[0] != '\0') {
        return error.message;
    }
    return fmt::format("{} failed with result {}", fnName, static_cast<int>(result));
}

static std::string native_status_message(const NativeModStatus status) {
    switch (status) {
    case NativeModStatus::BuildDisabled:
        return "Code mods are disabled on this Dusklight build";
    case NativeModStatus::ModMissingPlatform:
        return fmt::format("Mod not supported on this platform ({})", k_nativeLibName);
    case NativeModStatus::ApiVersionMismatch:
        // TODO: differentiate whether mod or Dusklight is out of date
        return "Mod ABI version mismatch";
    case NativeModStatus::MissingExport:
        return "Missing required mod API exports";
    case NativeModStatus::Unknown:
        return "Unknown mod load failure";
    case NativeModStatus::None:
    case NativeModStatus::Loaded:
        break;
    }
    return "native mod failed to load";
}

std::filesystem::path ModLoader::external_native_lib_path(const LoadedMod& mod) const {
    namespace fs = std::filesystem;
    if (k_nativeLibName.empty()) {
        return {};
    }
    const auto& libDir = m_searchDirs[mod.searchDirIndex].nativeLibDir;
    if (libDir.empty()) {
        return {};
    }
    fs::path path = libDir / fs::path(mod.metadata.id +
                                      io::fs_path_to_string(fs::path(k_nativeLibName).extension()));
    std::error_code ec;
    if (!fs::is_regular_file(path, ec)) {
        return {};
    }
    return path;
}

void ModLoader::load_native(LoadedMod& mod, const std::string& dllEntry) {
    if (!EnableCodeMods) {
        log::write(mod.metadata.id, LOG_LEVEL_ERROR, "Code mods are not available in this build");
        mod.nativeStatus = NativeModStatus::BuildDisabled;
        return;
    }

    namespace fs = std::filesystem;

    const fs::path cacheDir = m_cacheDir / mod.metadata.id;
    std::error_code ec;
    fs::create_directories(cacheDir, ec);

    fs::path libPath;
    if (mod.inPlace) {
        if (!dllEntry.empty()) {
            libPath = fs::path(mod.modPath) / dllEntry;
        } else if (auto external = external_native_lib_path(mod); !external.empty()) {
            libPath = std::move(external);
        } else {
            log::write(mod.metadata.id, LOG_LEVEL_ERROR,
                "no native library named {} found; skipping", k_nativeLibName);
            mod.nativeStatus = NativeModStatus::ModMissingPlatform;
            return;
        }
    } else {
        if (dllEntry.empty()) {
            log::write(mod.metadata.id, LOG_LEVEL_ERROR,
                "no native library named {} found; skipping", k_nativeLibName);
            mod.nativeStatus = NativeModStatus::ModMissingPlatform;
            return;
        }

        // Generation-versioned filename: every dlopen gets a path it has never seen, so a reload
        // always yields a fresh image with fresh statics even if the previous dlclose did not
        // fully unmap the old one (TLS/ObjC pinning). The .cache dir is wiped on startup.
        const fs::path dllCachePath =
            cacheDir / fmt::format("{}.g{}{}", mod.metadata.id, ++mod.cacheGeneration,
                           io::fs_path_to_string(fs::path(dllEntry).extension()));

        std::vector<u8> dllData;
        try {
            dllData = mod.bundle->readFile(dllEntry);
        } catch (const std::exception& e) {
            log::write(mod.metadata.id, LOG_LEVEL_ERROR, "failed to extract {}", dllEntry);
            return;
        }

        {
            std::ofstream out(dllCachePath, std::ios::binary | std::ios::out);
            if (!out) {
                log::write(mod.metadata.id, LOG_LEVEL_ERROR, "failed to write {}",
                    io::fs_path_to_string(dllCachePath));
                return;
            }

            out.write(reinterpret_cast<const char*>(dllData.data()),
                static_cast<std::streamsize>(dllData.size()));
        }

        libPath = dllCachePath;
    }

    auto nativeMod = std::make_unique<NativeMod>();
    try {
        nativeMod->handle = std::make_unique<loader::NativeModule>(libPath);
    } catch (const std::runtime_error& e) {
        log::write(mod.metadata.id, LOG_LEVEL_ERROR, "failed to open {}: {}",
            io::fs_path_to_string(libPath), e.what());
        return;
    }

    const auto getManifest = nativeMod->handle->LookupSymbol<ModGetManifestFn>("mod_get_manifest");
    nativeMod->contextSymbol = nativeMod->handle->LookupSymbol<ModContext**>("mod_ctx");
    nativeMod->fn_initialize = nativeMod->handle->LookupSymbol<ModInitializeFn>("mod_initialize");
    nativeMod->fn_update = nativeMod->handle->LookupSymbol<ModUpdateFn>("mod_update");
    nativeMod->fn_shutdown = nativeMod->handle->LookupSymbol<ModShutdownFn>("mod_shutdown");

    if (!getManifest || !nativeMod->contextSymbol || !nativeMod->fn_initialize ||
        !nativeMod->fn_update || !nativeMod->fn_shutdown)
    {
        log::write(mod.metadata.id, LOG_LEVEL_ERROR,
            "{} missing required mod API exports; skipping",
            io::fs_path_to_string(libPath.filename()));
        mod.nativeStatus = NativeModStatus::MissingExport;
        return;
    }

    nativeMod->manifest = getManifest();
    if (!validate_manifest(nativeMod->manifest, mod)) {
        return;
    }

    if (!validate_context_symbol(nativeMod->contextSymbol, mod)) {
        return;
    }
    *nativeMod->contextSymbol = mod.context.get();

    mod.dir = io::fs_path_to_string(fs::absolute(cacheDir));
    mod.nativePath = io::fs_path_to_string(fs::absolute(libPath));
    mod.native = std::move(nativeMod);
    mod.nativeStatus = NativeModStatus::Loaded;
}

void ModLoader::unload_native(LoadedMod& mod) {
    if (!mod.native || mod.inPlace) {
        return;
    }
    // Deferred dlclose: this mod's code may still be on the stack below the current tick
    m_retiredNatives.push_back({std::move(mod.native), std::move(mod.nativePath)});
    mod.nativePath.clear();
}

void ModLoader::drain_retired_natives() {
    for (auto& retired : m_retiredNatives) {
        retired.native.reset();
        if (!retired.path.empty()) {
            std::error_code ec;
            std::filesystem::remove(retired.path, ec);
        }
    }
    m_retiredNatives.clear();
}

static ModManifestInfo build_manifest_info(const ModManifest& manifest) {
    ModManifestInfo info;
    info.imports.reserve(manifest.import_count);
    for (size_t i = 0; i < manifest.import_count; ++i) {
        const auto& serviceImport = manifest.imports[i];
        if (serviceImport.struct_size != sizeof(ServiceImport) ||
            !svc::valid_service_id(serviceImport.service_id))
        {
            continue;
        }
        info.imports.push_back({serviceImport.service_id, serviceImport.major_version,
            (serviceImport.flags & SERVICE_IMPORT_OPTIONAL) == 0});
    }
    info.exports.reserve(manifest.export_count);
    for (size_t i = 0; i < manifest.export_count; ++i) {
        const auto& serviceExport = manifest.exports[i];
        if (serviceExport.struct_size != sizeof(ServiceExport) ||
            !svc::valid_service_id(serviceExport.service_id))
        {
            continue;
        }
        info.exports.push_back({serviceExport.service_id, serviceExport.major_version});
    }
    return info;
}

std::string escape_mod_id_for_config(std::string_view const id) {
    std::string buf;

    // Simple escaping. All characters in mod IDs literal, except for '.' and '_'.
    // '.' -> '_', '_' -> '__'
    for (char const chr : id) {
        if (chr == '.') {
            buf.push_back('_');
        } else if (chr == '_') {
            buf.push_back('_');
            buf.push_back('_');
        } else {
            buf.push_back(chr);
        }
    }

    return buf;
}

static std::string mod_enabled_cvar_name(std::string_view const id) {
    return fmt::format("mod.{}.enabled", escape_mod_id_for_config(id));
}

static bool required_deps_active(const LoadedMod& mod) {
    return std::ranges::all_of(mod.dependencies,
        [](const ModDependencyEdge& edge) { return !edge.required || edge.mod->active; });
}

// A deferred export that was not published by the end of the provider's initialization can
// never resolve, which is almost certainly a bug in the provider.
static void warn_unpublished_deferred_exports(const LoadedMod& mod) {
    if (!mod.active || !mod.native || mod.native->manifest == nullptr) {
        return;
    }

    const auto& manifest = *mod.native->manifest;
    for (size_t i = 0; i < manifest.export_count; ++i) {
        const auto& serviceExport = manifest.exports[i];
        if (serviceExport.struct_size != sizeof(ServiceExport) ||
            (serviceExport.flags & SERVICE_EXPORT_DEFERRED) == 0)
        {
            continue;
        }
        const auto* record =
            svc::find_service_record(serviceExport.service_id, serviceExport.major_version);
        if (record != nullptr && record->service == nullptr) {
            log::write(mod.metadata.id, LOG_LEVEL_WARN,
                "declared deferred service '{}@{}' but never published it during initialization",
                serviceExport.service_id, serviceExport.major_version);
        }
    }
}

void ModLoader::try_load_mod(
    const std::filesystem::path& modPath, bool fromDir, uint32_t searchDirIndex) {
    namespace fs = std::filesystem;

    std::unique_ptr<ModBundle> bundle;
    try {
        bundle = load_bundle(modPath, fromDir);
    } catch (const std::exception& e) {
        Log.error(
            "Failed to open {} bundle: {}", io::fs_path_to_string(modPath.filename()), e.what());
        return;
    }

    ModMetadata metadata;
    try {
        metadata = load_metadata(modPath, *bundle);
    } catch (const std::exception& e) {
        Log.error("bad mod.json in {}: {}", io::fs_path_to_string(modPath.filename()), e.what());
        return;
    }

    if (const auto* existing = find_mod(metadata.id)) {
        if (existing->searchDirIndex < searchDirIndex) {
            log::write(metadata.id, LOG_LEVEL_INFO, "{} shadowed by higher-priority copy {}",
                io::fs_path_to_string(modPath.filename()), existing->modPath);
        } else {
            log::write(metadata.id, LOG_LEVEL_ERROR, "duplicate mod id, not loading {}",
                io::fs_path_to_string(modPath.filename()));
        }
        return;
    }

    const auto& inserted = m_mods.emplace_back(std::make_unique<LoadedMod>());

    auto& mod = *inserted;
    mod.active = true;
    mod.modPath = io::fs_path_to_string(fs::absolute(modPath));
    mod.searchDirIndex = searchDirIndex;
    mod.inPlace = m_searchDirs[searchDirIndex].inPlaceNative && fromDir;
    mod.metadata = std::move(metadata);
    mod.bundle = std::move(bundle);
    mod.context = std::make_unique<ModContext>();
    mod.context->mod = &mod;
    mod.cvarIsEnabled =
        std::make_unique<ConfigVar<bool>>(mod_enabled_cvar_name(mod.metadata.id), true);

    const auto [dllEntry, anyLibs] = locate_dll_in_bundle(*mod.bundle);
    if (anyLibs || (mod.inPlace && !external_native_lib_path(mod).empty())) {
        mod.nativeStatus = NativeModStatus::Unknown;
        load_native(mod, dllEntry);
        if (mod.nativeStatus != NativeModStatus::Loaded) {
            Log.error("Native mod '{}' failed to load, disabling", mod.metadata.id);
            fail_mod(mod, MOD_ERROR, native_status_message(mod.nativeStatus));
        } else {
            mod.manifestInfo = build_manifest_info(*mod.native->manifest);
        }
    }

    log::write(mod.metadata.id, LOG_LEVEL_INFO, "found '{}' v{} by {} ({})", mod.metadata.name,
        mod.metadata.version, mod.metadata.author, io::fs_path_to_string(modPath.filename()));
}

bool ModLoader::activate_mod(LoadedMod& mod) {
    log::write(mod.metadata.id, LOG_LEVEL_INFO, "activating mod");
    mod.active = true;

    // Asset-only mods have no lifecycle beyond their overlay files.
    if (!mod.native) {
        mod.enabledApplied = true;
        return true;
    }

    if (!mod.servicesRegistered) {
        if (!register_static_service_exports(mod)) {
            log::write(mod.metadata.id, LOG_LEVEL_ERROR, "failed to register service exports");
            deactivate_mod(mod);
            return false;
        }
        mod.servicesRegistered = true;
    }

    if (!resolve_service_imports(mod)) {
        log::write(mod.metadata.id, LOG_LEVEL_ERROR, "failed to resolve service imports");
        deactivate_mod(mod);
        return false;
    }

    *mod.native->contextSymbol = mod.context.get();

    log::write(mod.metadata.id, LOG_LEVEL_TRACE, "calling mod_initialize");
    try {
        ModError error = MOD_ERROR_INIT;
        const auto result = mod.native->fn_initialize(&error);
        if (result == MOD_OK && !mod.loadFailed) {
            mod.initialized = true;
            log::write(mod.metadata.id, LOG_LEVEL_TRACE, "mod_initialize succeeded");
        } else if (result != MOD_OK && !mod.loadFailed) {
            fail_mod(mod, result, lifecycle_error_message("mod_initialize", result, error));
        }
    } catch (const std::exception& e) {
        fail_mod(mod, MOD_ERROR, fmt::format("Exception in mod_initialize: {}", e.what()));
    } catch (...) {
        fail_mod(mod, MOD_ERROR, "Unknown exception in mod_initialize");
    }

    warn_unpublished_deferred_exports(mod);

    if (!mod.active) {
        // Failed initialization may have left hooks or other service state behind
        deactivate_mod(mod);
        return false;
    }

    mod.enabledApplied = true;
    return true;
}

void ModLoader::deactivate_mod(LoadedMod& mod) {
    if (mod.initialized && mod.native && mod.native->fn_shutdown) {
        log::write(mod.metadata.id, LOG_LEVEL_TRACE, "calling mod_shutdown");
        try {
            ModError error = MOD_ERROR_INIT;
            const auto result = mod.native->fn_shutdown(&error);
            if (result == MOD_OK) {
                log::write(mod.metadata.id, LOG_LEVEL_TRACE, "mod_shutdown succeeded");
            } else {
                log::write(mod.metadata.id, LOG_LEVEL_ERROR, "mod_shutdown failed: {}",
                    lifecycle_error_message("mod_shutdown", result, error));
            }
        } catch (...) {
        }
    }
    mod.initialized = false;

    if (mod.servicesRegistered) {
        svc::remove_services_for_provider(mod);
        mod.servicesRegistered = false;
    }
    svc::modules_mod_detached(mod);
    unload_native(mod);

    mod.active = false;
    mod.enabledApplied = false;
}

void ModLoader::init() {
    if (m_initialized) {
        return;
    }
    m_initialized = true;

    manifest::initialize();

    if (m_searchDirs.empty()) {
        Log.warn("no mod search directories configured; mod loading skipped");
        return;
    }

    if (m_cacheDir.empty()) {
        m_cacheDir = m_searchDirs.front().path / ".cache";
    }

    namespace fs = std::filesystem;
    std::error_code ec;

    // Stale libs from previous sessions (see load_native).
    fs::remove_all(m_cacheDir, ec);

    for (size_t dirIndex = 0; dirIndex < m_searchDirs.size(); ++dirIndex) {
        const auto& searchDir = m_searchDirs[dirIndex];

        // --mods can point the user dir at the bundled dir; don't scan the same dir twice.
        bool alreadyScanned = false;
        for (size_t earlier = 0; earlier < dirIndex && !alreadyScanned; ++earlier) {
            alreadyScanned = fs::equivalent(m_searchDirs[earlier].path, searchDir.path, ec);
        }
        if (alreadyScanned) {
            continue;
        }

        if (!fs::is_directory(searchDir.path)) {
            if (dirIndex == 0) {
                Log.info("mods directory '{}' not found", io::fs_path_to_string(searchDir.path));
            } else {
                Log.debug("mods directory '{}' not found", io::fs_path_to_string(searchDir.path));
            }
            continue;
        }

        std::vector<fs::directory_entry> entries;
        for (auto& e : fs::directory_iterator(searchDir.path, ec)) {
            if (e.is_directory() && std::filesystem::exists(e.path() / "mod.json")) {
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
            try_load_mod(entry.path(), entry.is_directory(), static_cast<uint32_t>(dirIndex));
        }
    }

    if (m_mods.empty()) {
        Log.info("no mods found");
        return;
    }

    std::stable_sort(m_mods.begin(), m_mods.end(),
        [](const auto& a, const auto& b) { return a->searchDirIndex > b->searchDirIndex; });

    Log.info("initializing {} mod(s)...", m_mods.size());
    for (auto& mod : mods()) {
        mod.enabledSubscription = Register(*mod.cvarIsEnabled,
            [this, &mod](const bool&, const bool&) { on_enabled_changed(mod); });
    }

    init_services();

    // Providers must initialize (and publish deferred services) before their importers, so
    // imports are resolved per mod, interleaved with initialization, in dependency order.
    loader::sort_mods(m_mods);

    // Decide the startup lifecycle state before publishing exports. Config-disabled mods and
    // mods blocked by required providers keep dependency edges but must not resolve or provide
    // services until they can actually initialize.
    for (auto& mod : mods()) {
        if (!mod.cvarIsEnabled->getValue()) {
            log::write(mod.metadata.id, LOG_LEVEL_INFO, "disabled by config");
            mod.active = false;
            mod.suspendedByProvider = false;
            continue;
        }
        if (!mod.loadFailed && !required_deps_active(mod)) {
            log::write(
                mod.metadata.id, LOG_LEVEL_INFO, "suspended: a required provider is disabled");
            mod.active = false;
            mod.suspendedByProvider = true;
            continue;
        }
        mod.suspendedByProvider = false;
    }

    for (auto& mod : mods()) {
        if (!mod.active || !mod.native) {
            continue;
        }
        if (register_static_service_exports(mod)) {
            mod.servicesRegistered = true;
        } else {
            log::write(mod.metadata.id, LOG_LEVEL_ERROR, "failed to register service exports");
            deactivate_mod(mod);
        }
    }

    for (auto& mod : mods()) {
        if (mod.active) {
            activate_mod(mod);
        }
    }

    svc::modules_lifecycle_applied();

    auto active = std::ranges::count_if(mods(), [](const LoadedMod& m) { return m.active; });
    Log.info("{}/{} mod(s) active", active, m_mods.size());

    m_startupComplete = true;
}

LoadedMod* ModLoader::find_mod(std::string_view id) const {
    for (auto& mod : mods()) {
        if (mod.metadata.id == id) {
            return &mod;
        }
    }
    return nullptr;
}

void ModLoader::request_enable(std::string_view id) {
    if (auto* mod = find_mod(id)) {
        mod->cvarIsEnabled->setValue(true);
    }
}

void ModLoader::request_disable(std::string_view id) {
    if (auto* mod = find_mod(id)) {
        mod->cvarIsEnabled->setValue(false);
    }
}

void ModLoader::request_reload(std::string_view id) {
    m_pendingRequests.push_back({std::string{id}, RequestKind::Reload});
}

void ModLoader::notify_mod_failure(LoadedMod& mod, bool firstFailure) {
    if (firstFailure) {
        m_pendingFailures.push_back(mod.metadata.name);
    }
    // Startup failures are handled inline by activate_mod
    if (!m_startupComplete) {
        return;
    }
    m_pendingRequests.push_back({mod.metadata.id, RequestKind::Disable});
}

void ModLoader::flush_toasts() {
    if (m_pendingFailures.empty()) {
        return;
    }

    const auto names = std::exchange(m_pendingFailures, {});

    // Skip displaying toasts if the mods window is currently open
    if (const auto* window = dynamic_cast<const ui::ModsWindow*>(ui::top_document())) {
        if (window->visible()) {
            return;
        }
    }

    ui::Toast toast{.type = "warning", .duration = std::chrono::seconds{5}};
    if (names.size() == 1) {
        toast.title = "Mod failed";
        toast.content =
            fmt::format("<div><b>{}</b> failed and was disabled.</div><div>Check Mods for "
                        "more information.</div>",
                ui::escape(names.front()));
    } else {
        toast.title = "Mods failed";
        toast.content = fmt::format("<div><b>{} mods</b> failed and were disabled.</div><div>Check "
                                    "Mods for more information.</div>",
            names.size());
    }
    ui::push_toast(std::move(toast));
}

std::vector<LoadedMod*> ModLoader::collect_lifecycle_set(LoadedMod& target) {
    std::vector included{&target};
    std::vector pending{&target};
    while (!pending.empty()) {
        auto* current = pending.back();
        pending.pop_back();
        for (const auto& edge : current->dependents) {
            auto* dependent = edge.mod;
            if (!dependent->active && !dependent->suspendedByProvider) {
                continue;
            }
            if (std::ranges::find(included, dependent) != included.end()) {
                continue;
            }
            included.push_back(dependent);
            pending.push_back(dependent);
        }
    }

    std::vector<LoadedMod*> ordered;
    ordered.reserve(included.size());
    for (auto& mod : mods()) {
        if (std::ranges::find(included, &mod) != included.end()) {
            ordered.push_back(&mod);
        }
    }
    return ordered;
}

bool ModLoader::ensure_native_loaded(LoadedMod& mod) {
    if (mod.native || mod.nativeStatus == NativeModStatus::None) {
        return true;
    }

    const auto [dllEntry, anyLibs] = locate_dll_in_bundle(*mod.bundle);
    if (!anyLibs && !(mod.inPlace && !external_native_lib_path(mod).empty())) {
        mod.nativeStatus = NativeModStatus::None;
        return true;
    }

    mod.nativeStatus = NativeModStatus::Unknown;
    load_native(mod, dllEntry);
    if (mod.nativeStatus != NativeModStatus::Loaded) {
        fail_mod(mod, MOD_ERROR, native_status_message(mod.nativeStatus));
        return false;
    }
    return true;
}

bool ModLoader::reload_bundle(LoadedMod& mod) {
    namespace fs = std::filesystem;
    log::write(mod.metadata.id, LOG_LEVEL_INFO, "reloading from {}", mod.modPath);

    std::shared_ptr<ModBundle> newBundle;
    ModMetadata newMetadata;
    try {
        std::error_code ec;
        newBundle = load_bundle(mod.modPath, fs::is_directory(mod.modPath, ec));
        newMetadata = load_metadata(mod.modPath, *newBundle);
    } catch (const std::exception& e) {
        fail_mod(mod, MOD_ERROR, fmt::format("Reload failed: {}", e.what()));
        return false;
    }

    if (newMetadata.id != mod.metadata.id) {
        fail_mod(mod, MOD_CONFLICT,
            fmt::format("Mod ID changed on reload ('{}'); restart required", newMetadata.id));
        return false;
    }

    mod.metadata = std::move(newMetadata);
    // In-flight readers of the old bundle keep it alive through their shared_ptr.
    mod.bundle = std::move(newBundle);
    mod.loadFailed = false;
    mod.failureReason.clear();

    ModManifestInfo newInfo;
    if (const auto [dllEntry, anyLibs] = locate_dll_in_bundle(*mod.bundle); anyLibs) {
        mod.nativeStatus = NativeModStatus::Unknown;
        load_native(mod, dllEntry);
        if (mod.nativeStatus != NativeModStatus::Loaded) {
            fail_mod(mod, MOD_ERROR, native_status_message(mod.nativeStatus));
            return false;
        }
        newInfo = build_manifest_info(*mod.native->manifest);
    } else {
        mod.nativeStatus = NativeModStatus::None;
        ++mod.cacheGeneration;
    }

    if (newInfo != mod.manifestInfo) {
        // The reload changes the mod's imports/exports; rebuild the dependency graph so edges,
        // init/tick/shutdown order and cascade sets reflect the new manifest.
        log::write(mod.metadata.id, LOG_LEVEL_INFO,
            "changed its service imports/exports; rebuilding mod dependency graph");
        mod.manifestInfo = std::move(newInfo);
        loader::sort_mods(m_mods);
    }

    return true;
}

void ModLoader::apply_lifecycle_change(LoadedMod& target, const bool reload) {
    auto affected = collect_lifecycle_set(target);

    // Dependents first (reverse init order), like shutdown.
    for (auto* mod : affected | std::views::reverse) {
        const bool needsTeardown =
            mod->active ||
            (mod == &target && (mod->initialized || (reload && mod->native != nullptr)));
        if (!needsTeardown) {
            continue;
        }
        const bool wasActive = mod->active;
        log::write(mod->metadata.id, LOG_LEVEL_INFO, "deactivating mod");
        deactivate_mod(*mod);
        if (mod != &target && wasActive) {
            // Provisional; cleared below if the mod comes straight back up.
            mod->suspendedByProvider = true;
        }
    }

    if (reload) {
        // On failure the target is failed and stays down; dependents get resume attempts below
        // and suspend against the failed provider where required.
        reload_bundle(target);

        // The reload may have rebuilt the dependency graph and reordered m_mods; refresh the
        // set's iteration order so reactivation still runs providers first.
        std::vector<LoadedMod*> reordered;
        reordered.reserve(affected.size());
        for (auto& mod : mods()) {
            if (std::ranges::find(affected, &mod) != affected.end()) {
                reordered.push_back(&mod);
            }
        }
        affected = std::move(reordered);
    }

    // Mirror startup: publish every candidate's static exports before any of them initialize,
    // so importers within the set resolve providers regardless of initialization order
    // (optional cycles rely on this).
    for (auto* mod : affected) {
        if (mod->active || mod->loadFailed || !mod->cvarIsEnabled->getValue()) {
            continue;
        }
        if (!ensure_native_loaded(*mod)) {
            continue;
        }
        if (mod->native && !mod->servicesRegistered) {
            if (register_static_service_exports(*mod)) {
                mod->servicesRegistered = true;
            } else {
                log::write(
                    mod->metadata.id, LOG_LEVEL_ERROR, "failed to register service exports");
                deactivate_mod(*mod);
            }
        }
    }

    // Providers first (init order). The target is naturally first among the affected mods.
    for (auto* mod : affected) {
        if (mod->active || mod->loadFailed || !mod->cvarIsEnabled->getValue()) {
            continue;
        }
        if (!required_deps_active(*mod)) {
            mod->suspendedByProvider = true;
            log::write(
                mod->metadata.id, LOG_LEVEL_INFO, "suspended: a required provider is disabled");
            continue;
        }
        mod->suspendedByProvider = false;
        activate_mod(*mod);
    }

    // Mods that stayed down must not leave their exports resolvable.
    for (auto* mod : affected) {
        if (!mod->active && mod->servicesRegistered) {
            svc::remove_services_for_provider(*mod);
            mod->servicesRegistered = false;
        }
    }
}

void ModLoader::on_enabled_changed(LoadedMod& mod) {
    svc::config_mark_dirty();
    if (mod.loadFailed) {
        if (!mod.cvarIsEnabled->getValue()) {
            mod.loadFailed = false;
            mod.failureReason.clear();
        }
        return;
    }
    if (mod.suspendedByProvider) {
        if (!mod.cvarIsEnabled->getValue()) {
            // The user disabled a suspended mod; stop waiting for its providers.
            mod.suspendedByProvider = false;
        }
        return;
    }
    m_pendingRequests.push_back({mod.metadata.id,
        mod.cvarIsEnabled->getValue() ? RequestKind::Enable : RequestKind::Disable});
}

void ModLoader::apply_pending_requests() {
    // Images retired by the previous tick have had a full frame to unwind off the stack.
    drain_retired_natives();

    if (m_pendingRequests.empty()) {
        return;
    }

    // Coalesce per mod, last request wins. Failures during apply re-enqueue for next tick.
    const auto requests = std::exchange(m_pendingRequests, {});
    std::vector<Request> coalesced;
    for (const auto& request : requests) {
        const auto existing = std::ranges::find_if(
            coalesced, [&](const Request& r) { return r.modId == request.modId; });
        if (existing != coalesced.end()) {
            existing->kind = request.kind;
        } else {
            coalesced.push_back(request);
        }
    }

    for (const auto& request : coalesced) {
        auto* mod = find_mod(request.modId);
        if (mod == nullptr) {
            Log.warn("lifecycle request for unknown mod '{}'", request.modId);
            continue;
        }
        if (request.kind == RequestKind::Reload && mod->inPlace) {
            log::write(mod->metadata.id, LOG_LEVEL_WARN, "is a built-in mod and can't be reloaded");
            continue;
        }
        if (request.kind == RequestKind::Enable && mod->enabledApplied) {
            continue;
        }
        if (request.kind == RequestKind::Disable && !mod->enabledApplied && !mod->active) {
            continue;
        }
        apply_lifecycle_change(*mod, request.kind == RequestKind::Reload);
    }

    svc::modules_lifecycle_applied();

    auto active = std::ranges::count_if(mods(), [](const LoadedMod& m) { return m.active; });
    Log.info("{}/{} mod(s) active", active, m_mods.size());
}

void ModLoader::tick() {
    svc::modules_frame_begin();
    apply_pending_requests();

    for (auto& mod : mods()) {
        if (!mod.active || !mod.native) {
            continue;
        }
        try {
            ModError error = MOD_ERROR_INIT;
            const auto result = mod.native->fn_update(&error);
            if (result != MOD_OK) {
                fail_mod(mod, result, lifecycle_error_message("mod_update", result, error));
            }
        } catch (const std::exception& e) {
            fail_mod(mod, MOD_ERROR, fmt::format("Exception in mod_update: {}", e.what()));
        } catch (...) {
            fail_mod(mod, MOD_ERROR, "Unknown exception in mod_update");
        }
    }

    svc::modules_frame_end();
    flush_toasts();
}

void ModLoader::shutdown() {
    // Reverse initialization order, so importers shut down before their service providers.
    for (auto& mod : mods() | std::views::reverse) {
        deactivate_mod(mod);
        if (mod.enabledSubscription != 0) {
            config::unsubscribe(mod.enabledSubscription);
            mod.enabledSubscription = 0;
        }
        unregister(*mod.cvarIsEnabled);
    }

    m_mods.clear();
    drain_retired_natives();
    svc::modules_shutdown();
    Log.info("all mods unloaded");
}

}  // namespace dusk::mods
