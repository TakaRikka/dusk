#ifndef DUSK_MAIN_H
#define DUSK_MAIN_H

#include <filesystem>

namespace dusk {

extern bool IsRunning;
extern bool IsShuttingDown;
extern bool IsGameLaunched;
extern bool RestartRequested;
extern std::filesystem::path ConfigPath;
extern std::filesystem::path CachePath;

#if defined(__ANDROID__) || (defined(TARGET_OS_IOS) && TARGET_OS_IOS) ||                           \
    (defined(TARGET_OS_TV) && TARGET_OS_TV)
inline constexpr bool SupportsProcessRestart = false;
#else
inline constexpr bool SupportsProcessRestart = true;
#endif

void RequestRestart() noexcept;

}  // namespace dusk

#endif  // DUSK_MAIN_H
