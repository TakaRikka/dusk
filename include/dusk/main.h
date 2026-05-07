#ifndef DUSK_MAIN_H
#define DUSK_MAIN_H

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

#include <filesystem>

namespace dusk {
    extern bool IsRunning;
    extern bool IsShuttingDown;
    extern bool IsGameLaunched;
    extern bool IsFocusPaused;
    extern bool RestartRequested;
    extern std::filesystem::path ConfigPath;

#if defined(__ANDROID__) || (defined(TARGET_OS_IOS) && TARGET_OS_IOS) || \
    (defined(TARGET_OS_TV) && TARGET_OS_TV)
    inline constexpr bool SupportsProcessRestart = false;
#else
    inline constexpr bool SupportsProcessRestart = true;
#endif

    void RequestRestart() noexcept;
}

#endif  // DUSK_MAIN_H
