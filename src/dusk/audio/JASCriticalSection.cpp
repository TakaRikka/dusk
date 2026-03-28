#include "JSystem/JAudio2/JASCriticalSection.h"

#include <mutex>

static std::recursive_mutex gAudioThreadMutex;

JASCriticalSection::JASCriticalSection() {
    gAudioThreadMutex.lock();
}

JASCriticalSection::~JASCriticalSection() {
    gAudioThreadMutex.unlock();
}
