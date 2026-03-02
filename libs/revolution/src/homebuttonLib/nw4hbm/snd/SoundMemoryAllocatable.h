#ifndef NW4HBM_SND_SOUND_MEMORY_ALLOCATABLE_H
#define NW4HBM_SND_SOUND_MEMORY_ALLOCATABLE_H

#include "snd_types.h"

namespace nw4hbm {
    namespace snd {
        class SoundMemoryAllocatable {
        public:
            /* 0x08 */ virtual ~SoundMemoryAllocatable() {}
            /* 0x0C */ virtual void* Alloc(u32 size) = 0;
        };
    }  // namespace snd
}  // namespace nw4hbm

#endif
