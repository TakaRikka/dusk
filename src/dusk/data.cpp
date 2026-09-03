#include "data.hpp"

#include "dusk/app_info.hpp"

#include <borealis/io.hpp>
#include <borealis/log.hpp>

#include <string>

namespace dusk::data {
namespace {

constexpr borealis::Log Log{"dusk::data"};

std::string status_message(const borealis::data::Status& status) {
    using enum borealis::data::ErrorCode;

    std::string message;
    switch (status.code) {
    case None:
        return {};
    case NotInitialized:
        message = "Data folders have not been initialized.";
        break;
    case PreferencePathUnavailable:
        message = "The system data folder is unavailable.";
        break;
    case EmptyPath:
        message = "Choose a folder.";
        break;
    case CreateDirectoryFailed:
        message = "The selected folder could not be created.";
        break;
    case NotDirectory:
        message = "The selected path is not a folder.";
        break;
    case WriteProbeFailed:
        message = "The selected folder is not writable.";
        break;
    case WriteProbeCleanupFailed:
        message = "The selected folder could not be validated.";
        break;
    case DescriptorWriteFailed:
        message = "Dusklight could not save the data folder setting.";
        break;
    case MigrationIncomplete:
        message = "Data migration could not be completed.";
        break;
    case OverrideActive:
        message = "The data folder is fixed by --user-dir for this session.";
        break;
    case Unsupported:
        message = "Changing the data folder is not supported on this platform.";
        break;
    case OpenFolderFailed:
        message = "The data folder could not be opened.";
        break;
    }

    if (!status.path.empty()) {
        message += " (" + borealis::io::fs_path_to_string(status.path) + ")";
    }
    if (status.systemError) {
        message += ": " + status.systemError.message();
    }
    return message;
}

bool operation_succeeded(const borealis::data::Status& status, std::string* errorOut = nullptr) {
    if (status) {
        return true;
    }
    const auto message = status_message(status);
    if (errorOut != nullptr) {
        *errorOut = message;
    }
    Log.warn("{}", message);
    return false;
}

}  // namespace

borealis::data::Manager& manager() {
    static borealis::data::Manager instance{
        AppInfo,
        {
            .defaultPath = {.useDocumentsOnIOS = true},
            .portableRelativePath = "data",
            .legacyApps =
                {
                    {.orgName = "TwilitRealm", .appName = "Dusk"},
                },
            .migration =
                {
                    .directories =
                        {
                            "texture_replacements",
                            "USA",
                            "EUR",
                            "JAP",
                        },
                    .files =
                        {
                            "achievements.json",
                            "config.json",
                            "controller_ports.dat",
                            "gamecontrollerdb.txt",
                            "imgui.ini",
                            "keyboard_bindings.dat",
                            "states.json",
                        },
                    .extensions = {".controller", ".gci"},
                    .filenamePatterns =
                        {
                            {.prefix = "MemoryCard", .suffix = ".raw"},
                        },
                },
        },
    };
    return instance;
}

Paths initialize_data(const std::filesystem::path& userDirectoryOverride) {
    const auto status = manager().initialize(userDirectoryOverride);
    if (!status && status.code != borealis::data::ErrorCode::MigrationIncomplete) {
        Log.fatal("Failed to initialize data folders: {}", status_message(status));
    }
    if (!status) {
        Log.warn("{} Migration will be retried on the next launch.", status_message(status));
    }
    return manager().paths();
}

std::filesystem::path base_path_relative(const std::filesystem::path& path) {
    return manager().base_path_relative(path);
}

std::filesystem::path configured_data_path() {
    return manager().configured_data_path();
}

std::filesystem::path cache_path() {
    return manager().paths().cachePath;
}

bool open_data_path() {
    return operation_succeeded(manager().open_active_data_path());
}

bool set_custom_data_path(const std::filesystem::path& path, std::string* errorOut) {
    return operation_succeeded(manager().set_custom_data_path(path), errorOut);
}

bool set_custom_data_path(const char* path, std::string* errorOut) {
    if (path == nullptr) {
        if (errorOut != nullptr) {
            *errorOut = "Choose a folder.";
        }
        return false;
    }
    return set_custom_data_path(borealis::io::fs_path_from_utf8(path), errorOut);
}

bool set_portable_data_path() {
    return operation_succeeded(manager().set_portable_data_path());
}

bool reset_data_path() {
    return operation_succeeded(manager().reset_data_path());
}

bool is_default_data_path() {
    return manager().is_default_data_path();
}

bool is_data_path_restart_pending() {
    return manager().is_data_path_restart_pending();
}

std::filesystem::path user_home_path() {
    return manager().user_home_path();
}

std::filesystem::path normalized_display_path(const std::filesystem::path& path) {
    return manager().normalized_display_path(path);
}

std::string abbreviated_path_string(const std::filesystem::path& path) {
    return manager().abbreviated_path_string(path);
}

}  // namespace dusk::data
