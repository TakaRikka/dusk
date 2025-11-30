#ifndef JASMUTEX_H
#define JASMUTEX_H

#include <dolphin/os.h>

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JASMutexLock {
    JASMutexLock(OSMutex* mutex) {
        mMutex = mutex;
        OSLockMutex(mutex);
    }
    ~JASMutexLock() { OSUnlockMutex(mMutex); }

    /* 0x0 */ OSMutex* mMutex;
};

#endif
