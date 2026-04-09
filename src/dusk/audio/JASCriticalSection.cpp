#include "JSystem/JAudio2/JASCriticalSection.h"

#include <mutex>

#include "tracy/Tracy.hpp"

static TracyLockable(std::recursive_mutex, gAudioThreadMutex);

JASCriticalSection::JASCriticalSection() {
    gAudioThreadMutex.lock();
}

JASCriticalSection::~JASCriticalSection() {
    gAudioThreadMutex.unlock();
}
