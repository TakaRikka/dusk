#ifndef NW4HBM_SND_SOUND_HEAP_H
#define NW4HBM_SND_SOUND_HEAP_H

#include <revolution/types.h>

#include "FrameHeap.h"
#include "SoundMemoryAllocatable.h"

#include "../ut/Lock.h"

#include <revolution/os.h>

namespace nw4hbm {
    namespace snd {

        class SoundHeap : public SoundMemoryAllocatable {
        public:
            SoundHeap();
            /* 0x08 */ virtual ~SoundHeap();

            /* 0x0C */ virtual void* Alloc(u32 size);
            void* Alloc(u32 size, detail::FrameHeap::FreeCallback callback, void* callbackArg);

            bool Create(void* base, u32 size);
            void Destroy();

            void Clear() {
                ut::AutoMutexLock lock(mMutex);
                mFrameHeap.Clear();
            }

            int SaveState();
            void LoadState(int id);

            bool IsValid() { return mFrameHeap.IsValid(); }

            int GetCurrentLevel() const {
                ut::detail::AutoLock<OSMutex> lock(mMutex);
                return mFrameHeap.GetCurrentLevel();
            }

            u32 GetFreeSize() const {
                ut::detail::AutoLock<OSMutex> lock(mMutex);
                return mFrameHeap.GetFreeSize();
            }

        private:
            static void DisposeCallbackFunc(void* buffer, u32 size, void* callbackArg);

            /* 0x00 */ mutable OSMutex mMutex;
            /* 0x1C */ detail::FrameHeap mFrameHeap;
        };

    }  // namespace snd
}  // namespace nw4hbm

#endif
