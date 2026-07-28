#pragma once

#include <string>

#ifdef DEVKITPRO
    #define PLATFORM_DKP

    #include "../utility/path.hpp"
#elif defined(_MSC_VER)
    #define PLATFORM_MSVC
#elif defined(__GNUC__) || defined(__GNUG__)
    #define PLATFORM_GCC
#elif defined(__clang__)
    #define PLATFORM_CLANG
#else 
    #error UNKNOWN PLATFORM 
#endif

namespace randomizer::utility::platform
{
    void Log(const std::string& str);

    bool Init();

    bool IsRunning();

    void waitForPlatformStop();

    void Shutdown();

#ifdef DEVKITPRO
    bool mountDeviceAndConvertPath(fspath& path);
#endif
}
