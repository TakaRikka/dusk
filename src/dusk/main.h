#pragma once

#include <filesystem>

namespace dusk {

extern bool IsRunning;
extern bool IsShuttingDown;
extern bool IsGameLaunched;
extern bool RestartRequested;
extern std::filesystem::path ConfigPath;
extern std::filesystem::path CachePath;

extern uint8_t SaveRequested;
struct StageRequest {
    std::string stage;
    bool set;
    s8 room;
    s16 point;
    s8 layer;
};
extern StageRequest StageRequested;



#if defined(__ANDROID__) || (defined(TARGET_OS_IOS) && TARGET_OS_IOS) ||                           \
    (defined(TARGET_OS_TV) && TARGET_OS_TV)
inline constexpr bool SupportsProcessRestart = false;
#else
inline constexpr bool SupportsProcessRestart = true;
#endif

void RequestRestart() noexcept;

}  // namespace dusk
