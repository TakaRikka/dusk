#include "data.hpp"

#include "dusk/app_info.hpp"
#include "dusk/io.hpp"
#include "dusk/logging.h"
#include "dusk/main.h"

#include <array>
#include <chrono>
#include <filesystem>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_misc.h>
#include <SDL3/SDL_stdinc.h>

#include "nlohmann/json.hpp"

namespace dusk::data {
namespace {

aurora::Module Log{"dusk::data"};

constexpr auto kLocationDescriptorName = "data_location.json";
constexpr auto kPipelineCacheName = "pipeline_cache.db";
constexpr auto kInitialPipelineCacheName = "initial_pipeline_cache.db";

constexpr std::array<std::string_view, 4> kUserDataDirectories = {
    "texture_replacements",
    "USA",
    "EUR",
    "JAP",
};
constexpr std::array<std::string_view, 6> kUserDataFiles = {
    "achievements.json",
    "config.json",
    "controller_ports.dat",
    "imgui.ini",
    "keyboard_bindings.dat",
    "states.json",
};

enum class LocationMode {
    Default,
    Portable,
    Custom,
};

struct LocationDescriptor {
    LocationMode mode = LocationMode::Default;
    std::filesystem::path customPath;
    std::filesystem::path previousPath;
};

struct LocatedDescriptor {
    LocationDescriptor descriptor;
    std::filesystem::path path;
};

struct MigrationStats {
    std::uintmax_t directoriesCreated = 0;
    std::uintmax_t filesCopied = 0;
    std::uintmax_t symlinksCopied = 0;
    std::uintmax_t sourcesRemoved = 0;
    std::uintmax_t emptyDirectoriesRemoved = 0;
    std::uintmax_t skippedExistingTargets = 0;
    std::uintmax_t skippedDescriptorFiles = 0;
    std::uintmax_t skippedNestedTargets = 0;
    std::uintmax_t skippedUnsupportedEntries = 0;
    std::uintmax_t failures = 0;
};

std::optional<std::filesystem::path> sConfiguredDataPath;
std::optional<std::filesystem::path> sActiveDescriptorPath;
std::optional<std::filesystem::path> sActivePrefPath;

std::filesystem::path path_from_utf8(std::string_view value) {
    return std::filesystem::path{
        reinterpret_cast<const char8_t*>(value.data()),
        reinterpret_cast<const char8_t*>(value.data() + value.size()),
    };
}

std::filesystem::path legacy_path_for_pref_path(const std::filesystem::path& prefPath) {
    if (std::string_view{LegacyAppName}.empty() || prefPath.empty()) {
        return {};
    }

    auto normalizedPrefPath = prefPath;
    if (normalizedPrefPath.filename().empty()) {
        normalizedPrefPath = normalizedPrefPath.parent_path();
    }

    const auto parentPath = normalizedPrefPath.parent_path();
    if (parentPath.empty()) {
        return {};
    }

    return parentPath / LegacyAppName;
}

std::filesystem::path get_pref_path() {
    char* prefPath = SDL_GetPrefPath(OrgName, AppName);
    if (!prefPath) {
        Log.fatal("Unable to get PrefPath: {}", SDL_GetError());
    }

    std::filesystem::path result{reinterpret_cast<const char8_t*>(prefPath)};
    SDL_free(prefPath);
    return result;
}

std::filesystem::path active_pref_path() {
    if (sActivePrefPath) {
        return *sActivePrefPath;
    }
    return get_pref_path();
}

std::filesystem::path base_path_relative(const std::filesystem::path& path) {
    const auto* basePath = SDL_GetBasePath();
    if (!basePath) {
        return path;
    }
    return std::filesystem::path{basePath} / path;
}

std::filesystem::path default_data_path(const std::filesystem::path& prefPath) {
#ifdef __APPLE__
#if TARGET_OS_IOS && !TARGET_OS_TV
    const char* documentsPath = SDL_GetUserFolder(SDL_FOLDER_DOCUMENTS);
    if (!documentsPath) {
        Log.fatal("Unable to get iOS Documents path: {}", SDL_GetError());
    }

    return reinterpret_cast<const char8_t*>(documentsPath);
#endif
#endif

    return prefPath;
}

std::filesystem::path portable_data_path() {
    return base_path_relative("data");
}

std::vector<std::filesystem::path> descriptor_paths(const std::filesystem::path& prefPath) {
    std::vector<std::filesystem::path> paths;
    if (const auto basePath = base_path_relative(kLocationDescriptorName); !basePath.empty()) {
        paths.push_back(basePath);
    }
    paths.push_back(prefPath / kLocationDescriptorName);
    return paths;
}

std::optional<LocationDescriptor> read_location_descriptor_file(const std::filesystem::path& path) {
    if (path.empty()) {
        return std::nullopt;
    }
    if (std::error_code ec; !std::filesystem::exists(path, ec)) {
        return std::nullopt;
    }

    try {
        const auto bytes = io::FileStream::ReadAllBytes(path);
        const auto json = nlohmann::json::parse(bytes);
        if (!json.is_object()) {
            Log.warn("Ignoring data location descriptor '{}': root is not an object",
                io::fs_path_to_string(path));
            return std::nullopt;
        }

        LocationDescriptor descriptor;
        const auto mode = json.value<std::string>("mode", "default");
        if (mode == "portable") {
            descriptor.mode = LocationMode::Portable;
        } else if (mode == "custom") {
            descriptor.mode = LocationMode::Custom;
        } else if (mode != "default") {
            Log.warn("Ignoring unknown data location mode '{}'", mode);
        }

        if (const auto customPath = json.find("customPath");
            customPath != json.end() && customPath->is_string())
        {
            descriptor.customPath = path_from_utf8(customPath->get<std::string>());
        }
        if (const auto previousPath = json.find("previousPath");
            previousPath != json.end() && previousPath->is_string())
        {
            descriptor.previousPath = path_from_utf8(previousPath->get<std::string>());
        }

        return descriptor;
    } catch (const std::exception& e) {
        Log.warn(
            "Ignoring data location descriptor '{}': {}", io::fs_path_to_string(path), e.what());
        return std::nullopt;
    }
}

std::optional<LocatedDescriptor> read_location_descriptor(const std::filesystem::path& prefPath) {
    for (const auto& path : descriptor_paths(prefPath)) {
        if (auto descriptor = read_location_descriptor_file(path)) {
            return LocatedDescriptor{
                .descriptor = *descriptor,
                .path = path,
            };
        }
    }
    return std::nullopt;
}

std::filesystem::path resolve_data_path(
    const std::filesystem::path& prefPath, const LocationDescriptor* descriptor) {
    if (!descriptor) {
        return default_data_path(prefPath);
    }

    switch (descriptor->mode) {
    case LocationMode::Default:
        return default_data_path(prefPath);
    case LocationMode::Portable:
        return portable_data_path();
    case LocationMode::Custom:
        if (!descriptor->customPath.empty()) {
            return descriptor->customPath;
        }
        Log.warn("Data location descriptor requested custom mode without a path");
        return default_data_path(prefPath);
    }

    return default_data_path(prefPath);
}

const char* location_mode_id(LocationMode mode) {
    switch (mode) {
    case LocationMode::Default:
        return "default";
    case LocationMode::Portable:
        return "portable";
    case LocationMode::Custom:
        return "custom";
    }

    return "default";
}

std::filesystem::path normalized_path(const std::filesystem::path& path) {
    std::error_code ec;
    auto normalized = std::filesystem::weakly_canonical(path, ec);
    if (!ec) {
        return normalized;
    }

    normalized = std::filesystem::absolute(path, ec);
    if (!ec) {
        return normalized.lexically_normal();
    }

    return path.lexically_normal();
}

std::filesystem::path absolute_path(const std::filesystem::path& path) {
    std::error_code ec;
    const auto absolute = std::filesystem::absolute(path, ec);
    if (ec) {
        return path;
    }
    return absolute.lexically_normal();
}

std::filesystem::path rename_legacy_pref_path(
    const std::filesystem::path& legacyPath, const std::filesystem::path& prefPath) {
    if (legacyPath.empty() || prefPath.empty() ||
        normalized_path(legacyPath) == normalized_path(prefPath))
    {
        return prefPath;
    }

    std::error_code ec;
    if (!std::filesystem::exists(legacyPath, ec)) {
        if (ec) {
            Log.warn("Failed to inspect legacy data directory '{}': {}",
                io::fs_path_to_string(legacyPath), ec.message());
        }
        return prefPath;
    }

    const bool prefExists = std::filesystem::exists(prefPath, ec);
    if (ec) {
        Log.warn("Failed to inspect data directory '{}': {}", io::fs_path_to_string(prefPath),
            ec.message());
        return prefPath;
    }
    if (prefExists) {
        if (!std::filesystem::is_directory(prefPath, ec) ||
            !std::filesystem::is_empty(prefPath, ec))
        {
            if (ec) {
                Log.warn("Failed to inspect data directory '{}': {}",
                    io::fs_path_to_string(prefPath), ec.message());
            } else {
                Log.info("Skipping legacy data directory rename because '{}' is not empty",
                    io::fs_path_to_string(prefPath));
            }
            return prefPath;
        }

        std::filesystem::remove(prefPath, ec);
        if (ec) {
            Log.warn("Failed to remove empty data directory '{}' before legacy rename: {}",
                io::fs_path_to_string(prefPath), ec.message());
            return prefPath;
        }
    }

    std::filesystem::rename(legacyPath, prefPath, ec);
    if (ec) {
        Log.warn("Failed to rename legacy data directory '{}' to '{}': {}",
            io::fs_path_to_string(legacyPath), io::fs_path_to_string(prefPath), ec.message());
        ec.clear();
        if (!std::filesystem::exists(prefPath, ec) && !ec) {
            Log.info("Using legacy data directory '{}' because the new data directory is absent",
                io::fs_path_to_string(legacyPath));
            return legacyPath;
        }
        return prefPath;
    }

    Log.info("Renamed legacy data directory '{}' to '{}'", io::fs_path_to_string(legacyPath),
        io::fs_path_to_string(prefPath));
    return prefPath;
}

bool is_same_or_inside(const std::filesystem::path& root, const std::filesystem::path& path) {
    const auto normalizedRoot = normalized_path(root);
    const auto normalizedPath = normalized_path(path);
    const auto relativePath = normalizedPath.lexically_relative(normalizedRoot);
    if (relativePath.empty()) {
        return normalizedPath == normalizedRoot;
    }
    if (relativePath == ".") {
        return true;
    }
    if (relativePath.is_absolute()) {
        return false;
    }

    const auto it = relativePath.begin();
    return it == relativePath.end() || *it != "..";
}

bool should_skip_migration_path(const std::filesystem::path& path,
    const std::filesystem::path& from, const std::filesystem::path& to, MigrationStats& stats) {
    if (is_same_or_inside(to, path)) {
        ++stats.skippedNestedTargets;
        return true;
    }

    const auto relativePath = path.lexically_relative(from);
    if (relativePath == kLocationDescriptorName) {
        ++stats.skippedDescriptorFiles;
        return true;
    }

    return false;
}

bool matches_name(std::string_view name, const auto& names) {
    return std::ranges::find(names, name) != names.end();
}

bool should_migrate_user_data_path(
    const std::filesystem::path& sourcePath, const std::filesystem::path& from) {
    const auto relativePath = sourcePath.lexically_relative(from);
    if (relativePath.empty() || relativePath.is_absolute()) {
        return false;
    }

    auto it = relativePath.begin();
    if (it == relativePath.end() || *it == "..") {
        return false;
    }

    const auto first = io::fs_path_to_string(*it);
    if (matches_name(first, kUserDataDirectories)) {
        return true;
    }

    ++it;
    if (it != relativePath.end()) {
        return false;
    }

    const auto filename = io::fs_path_to_string(relativePath.filename());
    if (matches_name(filename, kUserDataFiles)) {
        return true;
    }

    return relativePath.extension() == ".controller" || relativePath.extension() == ".gci" ||
           (filename.starts_with("MemoryCard") && filename.ends_with(".raw"));
}

std::filesystem::path current_data_path() {
    if (!ConfigPath.empty()) {
        return ConfigPath;
    }
    const auto prefPath = active_pref_path();
    const auto descriptor = read_location_descriptor(prefPath);
    if (descriptor) {
        sActiveDescriptorPath = descriptor->path;
    }
    return resolve_data_path(prefPath, descriptor ? &descriptor->descriptor : nullptr);
}

std::vector<std::filesystem::path> descriptor_write_paths(const std::filesystem::path& prefPath) {
    if (sActiveDescriptorPath && !sActiveDescriptorPath->empty()) {
        return {*sActiveDescriptorPath};
    }

    std::vector<std::filesystem::path> paths;
#if defined(_WIN32)
    if (const auto basePath = base_path_relative(kLocationDescriptorName); !basePath.empty()) {
        paths.push_back(basePath);
    }
#endif
    paths.push_back(prefPath / kLocationDescriptorName);
    return paths;
}

bool write_descriptor_json(const std::filesystem::path& path, const nlohmann::json& json) {
    std::error_code ec;
    std::filesystem::create_directories(path.parent_path(), ec);
    if (ec) {
        Log.warn("Failed to create data location descriptor directory '{}': {}",
            io::fs_path_to_string(path.parent_path()), ec.message());
        return false;
    }
    try {
        io::FileStream::WriteAllText(path, json.dump(4));
    } catch (const std::exception& e) {
        Log.warn("Failed to write data location descriptor '{}': {}", io::fs_path_to_string(path),
            e.what());
        return false;
    }
    return true;
}

bool write_location_descriptor(LocationMode mode, const std::filesystem::path& targetPath) {
    LocationDescriptor descriptor;
    descriptor.mode = mode;
    if (mode == LocationMode::Custom) {
        descriptor.customPath = absolute_path(targetPath);
    }

    const auto currentPath = current_data_path();
    const auto resolvedTargetPath =
        mode == LocationMode::Custom ? descriptor.customPath : targetPath;
    if (!currentPath.empty() && normalized_path(currentPath) != normalized_path(resolvedTargetPath))
    {
        descriptor.previousPath = currentPath;
    }

    nlohmann::json json;
    json["version"] = 1;
    json["mode"] = location_mode_id(descriptor.mode);
    if (descriptor.mode == LocationMode::Custom && !descriptor.customPath.empty()) {
        json["customPath"] = io::fs_path_to_string(descriptor.customPath);
    }
    if (!descriptor.previousPath.empty()) {
        json["previousPath"] = io::fs_path_to_string(descriptor.previousPath);
    }

    const auto prefPath = active_pref_path();
    for (const auto& path : descriptor_write_paths(prefPath)) {
        if (write_descriptor_json(path, json)) {
            sActiveDescriptorPath = path;
            sConfiguredDataPath = resolvedTargetPath;
            return true;
        }
    }

    return false;
}

void set_error(std::string* errorOut, std::string error) {
    if (errorOut != nullptr) {
        *errorOut = std::move(error);
    }
}

bool validate_writable_data_path(const std::filesystem::path& path, std::string* errorOut) {
    if (path.empty()) {
        set_error(errorOut, "Choose a folder.");
        return false;
    }

    std::error_code ec;
    std::filesystem::create_directories(path, ec);
    if (ec) {
        set_error(errorOut, fmt::format("{} could not create the selected folder.", AppName));
        Log.warn("Failed to create custom data folder '{}': {}", io::fs_path_to_string(path),
            ec.message());
        return false;
    }

    if (!std::filesystem::is_directory(path, ec)) {
        set_error(errorOut, "The selected path is not a folder.");
        if (ec) {
            Log.warn("Failed to inspect custom data folder '{}': {}", io::fs_path_to_string(path),
                ec.message());
        }
        return false;
    }

    const auto probePath = path / fmt::format(".write-probe-{}.tmp",
                                      std::chrono::steady_clock::now().time_since_epoch().count());
    try {
        io::FileStream::WriteAllText(probePath, "dusk");
    } catch (const std::exception& e) {
        set_error(errorOut, fmt::format("{} could not write to the selected folder.", AppName));
        Log.warn("Failed write probe for custom data folder '{}': {}", io::fs_path_to_string(path),
            e.what());
        return false;
    }

    std::filesystem::remove(probePath, ec);
    if (ec) {
        set_error(
            errorOut, fmt::format("{} could write to the selected folder, but could not remove "
                                  "the test file it created.",
                          AppName));
        Log.warn("Failed to remove custom data folder write probe '{}': {}",
            io::fs_path_to_string(probePath), ec.message());
        return false;
    }

    return true;
}

std::uintmax_t remove_empty_directories(const std::filesystem::path& root, bool includeRoot) {
    std::error_code ec;
    std::vector<std::filesystem::path> directories;
    for (std::filesystem::recursive_directory_iterator it(
             root, std::filesystem::directory_options::skip_permission_denied, ec);
        it != std::filesystem::recursive_directory_iterator(); it.increment(ec))
    {
        if (ec) {
            Log.warn("Failed to scan empty directories under '{}': {}", io::fs_path_to_string(root),
                ec.message());
            return 0;
        }
        const auto status = it->symlink_status(ec);
        if (ec) {
            Log.warn("Failed to inspect '{}' while pruning empty directories: {}",
                io::fs_path_to_string(it->path()), ec.message());
            ec.clear();
            continue;
        }
        if (std::filesystem::is_directory(status)) {
            directories.push_back(it->path());
        }
    }

    std::uintmax_t removed = 0;
    for (auto& dir : std::views::reverse(directories)) {
        if (!std::filesystem::is_empty(dir, ec)) {
            ec.clear();
            continue;
        }
        if (std::filesystem::remove(dir, ec)) {
            ++removed;
        } else if (ec) {
            Log.warn("Failed to remove empty migrated source directory '{}': {}",
                io::fs_path_to_string(dir), ec.message());
        }
        ec.clear();
    }

    if (includeRoot) {
        if (std::filesystem::is_empty(root, ec)) {
            if (std::filesystem::remove(root, ec)) {
                ++removed;
            } else if (ec) {
                Log.warn("Failed to remove empty migrated source root '{}': {}",
                    io::fs_path_to_string(root), ec.message());
            }
        }
        ec.clear();
    }

    return removed;
}

bool ensure_parent_directory(const std::filesystem::path& targetPath, MigrationStats& stats) {
    std::error_code ec;
    std::filesystem::create_directories(targetPath.parent_path(), ec);
    if (ec) {
        ++stats.failures;
        Log.warn("Failed to create migration target parent '{}': {}",
            io::fs_path_to_string(targetPath.parent_path()), ec.message());
        return false;
    }
    return true;
}

bool remove_migrated_source(const std::filesystem::path& sourcePath, MigrationStats& stats) {
    std::error_code ec;
    std::filesystem::remove(sourcePath, ec);
    if (ec) {
        ++stats.failures;
        Log.warn("Migrated '{}' but failed to remove source: {}", io::fs_path_to_string(sourcePath),
            ec.message());
        return false;
    }

    ++stats.sourcesRemoved;
    return true;
}

bool try_rename_migration_entry(
    const std::filesystem::path& sourcePath, const std::filesystem::path& targetPath) {
    std::error_code ec;
    if (std::filesystem::exists(targetPath, ec) || std::filesystem::is_symlink(targetPath, ec)) {
        return false;
    }
    ec.clear();

    if (!std::filesystem::exists(sourcePath, ec)) {
        return false;
    }
    ec.clear();

    std::filesystem::create_directories(targetPath.parent_path(), ec);
    if (ec) {
        Log.debug("Could not create migration target parent '{}' before rename: {}",
            io::fs_path_to_string(targetPath.parent_path()), ec.message());
        return false;
    }

    std::filesystem::rename(sourcePath, targetPath, ec);
    if (ec) {
        Log.debug("Could not rename migration entry '{}' to '{}': {}",
            io::fs_path_to_string(sourcePath), io::fs_path_to_string(targetPath), ec.message());
        return false;
    }

    return true;
}

void migrate_symlink(const std::filesystem::path& sourcePath,
    const std::filesystem::path& targetPath, MigrationStats& stats) {
    std::error_code ec;
    if (std::filesystem::exists(targetPath, ec) || std::filesystem::is_symlink(targetPath, ec)) {
        ++stats.skippedExistingTargets;
        return;
    }
    ec.clear();

    const auto linkTarget = std::filesystem::read_symlink(sourcePath, ec);
    if (ec) {
        ++stats.failures;
        Log.warn("Failed to read migration symlink '{}': {}", io::fs_path_to_string(sourcePath),
            ec.message());
        return;
    }

    if (!ensure_parent_directory(targetPath, stats)) {
        return;
    }

    const bool targetIsDirectory = std::filesystem::is_directory(sourcePath, ec);
    if (ec) {
        Log.debug("Could not resolve symlink target type for '{}': {}",
            io::fs_path_to_string(sourcePath), ec.message());
        ec.clear();
    }

    if (targetIsDirectory) {
        std::filesystem::create_directory_symlink(linkTarget, targetPath, ec);
    } else {
        std::filesystem::create_symlink(linkTarget, targetPath, ec);
    }
    if (ec) {
        ++stats.failures;
        Log.warn("Failed to migrate symlink '{}' -> '{}' to '{}': {}",
            io::fs_path_to_string(sourcePath), io::fs_path_to_string(linkTarget),
            io::fs_path_to_string(targetPath), ec.message());
        return;
    }

    ++stats.symlinksCopied;
    remove_migrated_source(sourcePath, stats);
}

void migrate_regular_file(const std::filesystem::path& sourcePath,
    const std::filesystem::path& targetPath, MigrationStats& stats) {
    std::error_code ec;
    if (std::filesystem::exists(targetPath, ec)) {
        ++stats.skippedExistingTargets;
        return;
    }
    ec.clear();

    if (try_rename_migration_entry(sourcePath, targetPath)) {
        ++stats.filesCopied;
        ++stats.sourcesRemoved;
        return;
    }

    if (!ensure_parent_directory(targetPath, stats)) {
        return;
    }

    std::filesystem::copy_file(
        sourcePath, targetPath, std::filesystem::copy_options::skip_existing, ec);
    if (ec) {
        ++stats.failures;
        Log.warn("Failed to migrate file '{}' to '{}': {}", io::fs_path_to_string(sourcePath),
            io::fs_path_to_string(targetPath), ec.message());
        return;
    }

    ++stats.filesCopied;
    remove_migrated_source(sourcePath, stats);
}

void migrate_directory(const std::filesystem::path& from, const std::filesystem::path& to,
    const std::filesystem::path& prefPath) {
    if (from.empty() || to.empty() || normalized_path(from) == normalized_path(to)) {
        Log.debug("Skipping data migration from '{}' to '{}'", io::fs_path_to_string(from),
            io::fs_path_to_string(to));
        return;
    }

    MigrationStats stats;

    std::error_code ec;
    if (!std::filesystem::exists(from, ec)) {
        if (ec) {
            Log.warn("Failed to inspect migration source '{}': {}", io::fs_path_to_string(from),
                ec.message());
        } else {
            Log.debug("Migration source '{}' does not exist", io::fs_path_to_string(from));
        }
        return;
    }

    std::filesystem::create_directories(to, ec);
    if (ec) {
        ++stats.failures;
        Log.warn("Failed to create data directory '{}' for migration: {}",
            io::fs_path_to_string(to), ec.message());
        return;
    }

    std::filesystem::recursive_directory_iterator it(
        from, std::filesystem::directory_options::skip_permission_denied, ec);
    if (ec) {
        Log.warn("Failed to begin migration scan for '{}': {}", io::fs_path_to_string(from),
            ec.message());
        return;
    }

    const std::filesystem::recursive_directory_iterator end;
    while (it != end) {
        if (ec) {
            ++stats.failures;
            Log.warn(
                "Migration scan error under '{}': {}", io::fs_path_to_string(from), ec.message());
            ec.clear();
        }

        const auto sourcePath = it->path();
        const auto status = it->symlink_status(ec);
        if (ec) {
            ++stats.failures;
            Log.warn("Failed to inspect migration source '{}': {}",
                io::fs_path_to_string(sourcePath), ec.message());
            ec.clear();
            it.increment(ec);
            continue;
        }

        if (should_skip_migration_path(sourcePath, from, to, stats)) {
            if (std::filesystem::is_directory(status)) {
                it.disable_recursion_pending();
            }
            ec.clear();
            it.increment(ec);
            continue;
        }

        if (!should_migrate_user_data_path(sourcePath, from)) {
            ++stats.skippedUnsupportedEntries;
            if (std::filesystem::is_directory(status)) {
                it.disable_recursion_pending();
            }
            ec.clear();
            it.increment(ec);
            continue;
        }

        const auto relativePath = sourcePath.lexically_relative(from);
        if (relativePath.empty() || relativePath.is_absolute()) {
            ++stats.failures;
            Log.warn("Failed to calculate migration relative path for '{}'",
                io::fs_path_to_string(sourcePath));
            it.increment(ec);
            continue;
        }

        const auto targetPath = to / relativePath;
        if (std::filesystem::is_symlink(status)) {
            migrate_symlink(sourcePath, targetPath, stats);
        } else if (std::filesystem::is_directory(status)) {
            if (try_rename_migration_entry(sourcePath, targetPath)) {
                ++stats.directoriesCreated;
                ++stats.sourcesRemoved;
                it.disable_recursion_pending();
            } else {
                std::filesystem::create_directories(targetPath, ec);
                if (ec) {
                    ++stats.failures;
                    Log.warn("Failed to create migration target directory '{}': {}",
                        io::fs_path_to_string(targetPath), ec.message());
                    ec.clear();
                    it.disable_recursion_pending();
                } else {
                    ++stats.directoriesCreated;
                }
            }
        } else if (std::filesystem::is_regular_file(status)) {
            migrate_regular_file(sourcePath, targetPath, stats);
        } else {
            ++stats.skippedUnsupportedEntries;
        }

        it.increment(ec);
    }

    const bool includeRoot = normalized_path(from) != normalized_path(prefPath);
    stats.emptyDirectoriesRemoved = remove_empty_directories(from, includeRoot);

    const bool migratedAnything = stats.filesCopied > 0 || stats.symlinksCopied > 0 ||
                                  stats.sourcesRemoved > 0 || stats.emptyDirectoriesRemoved > 0 ||
                                  stats.failures > 0;
    if (migratedAnything) {
        Log.info(
            "Finished data migration from '{}' to '{}': {} files copied, {} symlinks copied, {} "
            "sources removed, {} empty directories removed, {} existing targets skipped, {} "
            "descriptor files skipped, {} nested destination paths skipped, {} unsupported entries "
            "skipped, {} failures",
            io::fs_path_to_string(from), io::fs_path_to_string(to), stats.filesCopied,
            stats.symlinksCopied, stats.sourcesRemoved, stats.emptyDirectoriesRemoved,
            stats.skippedExistingTargets, stats.skippedDescriptorFiles, stats.skippedNestedTargets,
            stats.skippedUnsupportedEntries, stats.failures);
    }
}

void migrate_data(const std::filesystem::path& prefPath, const std::filesystem::path& dataPath,
    const LocationDescriptor* descriptor) {
    if (descriptor && !descriptor->previousPath.empty()) {
        migrate_directory(descriptor->previousPath, dataPath, prefPath);
    }
}

void ensure_data_directory(const std::filesystem::path& dataPath) {
    std::error_code ec;
    std::filesystem::create_directories(dataPath, ec);
    if (ec) {
        Log.fatal("Failed to create data directory '{}': {}", io::fs_path_to_string(dataPath),
            ec.message());
    }
}

SDL_IOStream* open_initial_pipeline_cache_source(std::string& sourcePathString) {
    const auto basePath = base_path_relative(kInitialPipelineCacheName);
    sourcePathString = io::fs_path_to_string(basePath);
    auto* source = SDL_IOFromFile(sourcePathString.c_str(), "rb");
    if (source != nullptr) {
        return source;
    }

    sourcePathString = std::string{kInitialPipelineCacheName};
    return SDL_IOFromFile(sourcePathString.c_str(), "rb");
}

void ensure_initial_pipeline_cache(const std::filesystem::path& configDir) {
    if (configDir.empty()) {
        return;
    }

    std::error_code ec;
    std::filesystem::create_directories(configDir, ec);
    if (ec) {
        Log.warn("Failed to create config directory '{}' for pipeline cache: {}",
            io::fs_path_to_string(configDir), ec.message());
        return;
    }

    const auto pipelineCachePath = configDir / kPipelineCacheName;
    if (std::filesystem::exists(pipelineCachePath, ec)) {
        return;
    }

    std::string sourcePathString;
    SDL_IOStream* source = open_initial_pipeline_cache_source(sourcePathString);
    if (source == nullptr) {
        Log.info("No bundled initial pipeline cache found");
        return;
    }

    const auto pipelineCacheString = io::fs_path_to_string(pipelineCachePath);
    SDL_IOStream* destination = SDL_IOFromFile(pipelineCacheString.c_str(), "wb");
    if (destination == nullptr) {
        Log.warn("Failed to open '{}' for seeded pipeline cache: {}", pipelineCacheString,
            SDL_GetError());
        SDL_CloseIO(source);
        return;
    }

    bool copied = true;
    std::array<char, 64 * 1024> buffer{};
    while (true) {
        const size_t bytesRead = SDL_ReadIO(source, buffer.data(), buffer.size());
        if (bytesRead > 0) {
            size_t bytesWritten = 0;
            while (bytesWritten < bytesRead) {
                const size_t written = SDL_WriteIO(
                    destination, buffer.data() + bytesWritten, bytesRead - bytesWritten);
                if (written == 0) {
                    Log.warn("Failed to write seeded pipeline cache '{}': {}", pipelineCacheString,
                        SDL_GetError());
                    copied = false;
                    break;
                }
                bytesWritten += written;
            }
        }

        if (!copied) {
            break;
        }

        if (bytesRead < buffer.size()) {
            if (SDL_GetIOStatus(source) == SDL_IO_STATUS_EOF) {
                break;
            }

            Log.warn(
                "Failed to read bundled pipeline cache '{}': {}", sourcePathString, SDL_GetError());
            copied = false;
            break;
        }
    }

    if (!SDL_CloseIO(destination)) {
        Log.warn(
            "Failed to close seeded pipeline cache '{}': {}", pipelineCacheString, SDL_GetError());
        copied = false;
    }
    SDL_CloseIO(source);

    if (!copied) {
        std::filesystem::remove(pipelineCachePath, ec);
        return;
    }

    Log.info("Seeded pipeline cache from '{}'", sourcePathString);
}

}  // namespace

bool open_data_path() {
#if DUSK_CAN_OPEN_DATA_FOLDER
    std::error_code ec;
    std::filesystem::path path = std::filesystem::absolute(ConfigPath, ec);
    if (ec) {
        Log.warn("Failed to resolve absolute data folder path '{}': {}",
            io::fs_path_to_string(ConfigPath), ec.message());
        path = ConfigPath;
    }

#if defined(_WIN32)
    const std::string url = "file:///" + path.generic_string();
#else
    const std::string url = "file://" + path.generic_string();
#endif
    if (!SDL_OpenURL(url.c_str())) {
        Log.warn(
            "Failed to open data folder '{}': {}", io::fs_path_to_string(path), SDL_GetError());
        return false;
    }
    return true;
#else
    return false;
#endif
}

bool set_custom_data_path(const std::filesystem::path& path, std::string* errorOut) {
    if (!validate_writable_data_path(path, errorOut)) {
        return false;
    }

    if (!write_location_descriptor(LocationMode::Custom, path)) {
        set_error(errorOut, fmt::format("{} could not save the data folder setting.", AppName));
        return false;
    }

    return true;
}

bool set_custom_data_path(const char* path, std::string* errorOut) {
    if (path == nullptr) {
        set_error(errorOut, "Choose a folder.");
        return false;
    }
    return set_custom_data_path(path_from_utf8(path), errorOut);
}

bool set_portable_data_path() {
    return write_location_descriptor(LocationMode::Portable, portable_data_path());
}

bool reset_data_path() {
    const auto prefPath = active_pref_path();
    return write_location_descriptor(LocationMode::Default, default_data_path(prefPath));
}

bool is_default_data_path() {
    const auto prefPath = active_pref_path();
    return normalized_path(configured_data_path()) == normalized_path(default_data_path(prefPath));
}

std::filesystem::path configured_data_path() {
    if (sConfiguredDataPath) {
        return *sConfiguredDataPath;
    }

    const auto prefPath = active_pref_path();
    const auto descriptor = read_location_descriptor(prefPath);
    if (descriptor) {
        sActiveDescriptorPath = descriptor->path;
    }
    sConfiguredDataPath =
        resolve_data_path(prefPath, descriptor ? &descriptor->descriptor : nullptr);
    return *sConfiguredDataPath;
}

std::filesystem::path cache_path() {
    if (!CachePath.empty()) {
        return CachePath;
    }
    return active_pref_path();
}

bool is_data_path_restart_pending() {
    if (ConfigPath.empty()) {
        return false;
    }

    return normalized_path(ConfigPath) != normalized_path(configured_data_path());
}

Paths initialize_data() {
    const auto preferredPrefPath = get_pref_path();
    const auto prefPath =
        rename_legacy_pref_path(legacy_path_for_pref_path(preferredPrefPath), preferredPrefPath);
    sActivePrefPath = prefPath;

    const auto descriptor = read_location_descriptor(prefPath);
    if (descriptor) {
        sActiveDescriptorPath = descriptor->path;
    } else {
        sActiveDescriptorPath.reset();
    }
    const auto dataPath =
        resolve_data_path(prefPath, descriptor ? &descriptor->descriptor : nullptr);
    sConfiguredDataPath = dataPath;

    migrate_data(prefPath, dataPath, descriptor ? &descriptor->descriptor : nullptr);
    ensure_data_directory(dataPath);
    ensure_data_directory(prefPath);
    ensure_initial_pipeline_cache(prefPath);

    return Paths{
        .userPath = dataPath,
        .cachePath = prefPath,
    };
}

}  // namespace dusk::data
