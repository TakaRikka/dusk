#ifndef DUSK_MAIN_H
#define DUSK_MAIN_H

#include <filesystem>

namespace dusk {
    extern bool IsRunning;
    extern bool IsShuttingDown;
    extern bool IsGameLaunched;
    extern bool IsFocusPaused;
    extern std::filesystem::path ConfigPath;
}

#endif  // DUSK_MAIN_H
