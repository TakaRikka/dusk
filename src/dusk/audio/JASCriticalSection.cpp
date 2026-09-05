#include "JSystem/JAudio2/JASCriticalSection.h"

#include <tracy/Tracy.hpp>

#include <mutex>

static TracyLockable(std::recursive_mutex, gAudioThreadMutex);

JASCriticalSection::JASCriticalSection() {
    gAudioThreadMutex.lock();
}

JASCriticalSection::~JASCriticalSection() {
    gAudioThreadMutex.unlock();
}
