#pragma once

#include <filesystem>

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

#if defined(_WIN32) ||                                                                             \
    (defined(__APPLE__) && !TARGET_OS_IOS && !TARGET_OS_TV && !TARGET_OS_MACCATALYST) ||           \
    (defined(__linux__) && !defined(__ANDROID__))
#define DUSK_CAN_OPEN_DATA_FOLDER 1
#else
#define DUSK_CAN_OPEN_DATA_FOLDER 0
#endif

#if defined(__APPLE__) && TARGET_OS_IOS && !TARGET_OS_MACCATALYST
#define DUSK_CAN_CHANGE_DATA_FOLDER 0
#else
#define DUSK_CAN_CHANGE_DATA_FOLDER 1
#endif

namespace dusk::data {

std::filesystem::path initialize_data();
std::filesystem::path configured_data_path();
bool open_data_path();
bool set_custom_data_path(const char* path);
bool set_custom_data_path(const std::filesystem::path& path);
bool set_portable_data_path();
bool reset_data_path();
bool is_default_data_path();
bool is_data_path_restart_pending();

}  // namespace dusk::data
