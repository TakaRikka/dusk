#pragma once

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

#if defined(__APPLE__) && TARGET_OS_IOS && !TARGET_OS_TV && !TARGET_OS_MACCATALYST
#define DUSK_HAS_IPHONE_HAPTICS 1
#else
#define DUSK_HAS_IPHONE_HAPTICS 0
#endif

namespace dusk::iphone_haptics {

#if DUSK_HAS_IPHONE_HAPTICS
bool isAvailable();
void start(float intensity);
void stop();
#else
inline bool isAvailable() {
    return false;
}

inline void start(float) {}
inline void stop() {}
#endif

}  // namespace dusk::iphone_haptics
