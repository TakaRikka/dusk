#ifndef NW4HBM_UT_LOCK_H
#define NW4HBM_UT_LOCK_H

#include "inlines.h"
#include <revolution/os.h>

namespace nw4hbm {
    namespace ut {
        namespace detail {

            inline void Lock(OSMutex& rMutex) {
                OSLockMutex(&rMutex);
            }
            inline void Unlock(OSMutex& rMutex) {
                OSUnlockMutex(&rMutex);
            }

            template <typename T>
            class AutoLock : private NonCopyable {
            public:
                explicit AutoLock(T& rLockObj) : mLockObj(rLockObj) { Lock(rLockObj); }
                ~AutoLock() { Unlock(mLockObj); }

            private:
                /* 0x00 */ T& mLockObj;
            };  // size = 0x04

        }  // namespace detail

        typedef detail::AutoLock<OSMutex> AutoMutexLock;

        class AutoInterruptLock : private NonCopyable {
        public:
            AutoInterruptLock() : mOldState(OSDisableInterrupts()) {}
            ~AutoInterruptLock() { OSRestoreInterrupts(mOldState); }

        private:
            /* 0x00 */ BOOL mOldState;
        };  // size = 0x04

    }  // namespace ut
}  // namespace nw4hbm

#endif
