#ifndef NW4HBM_SND_STRM_CHANNEL_H
#define NW4HBM_SND_STRM_CHANNEL_H

#include "snd_types.h"

namespace nw4hbm {
    namespace snd {
        namespace detail {

            typedef struct StrmChannel {
                /* 0x00 */ void* mBuffer;
                /* 0x04 */ u32 mBufferSize;
                /* 0x08 */ AdpcmInfo mAdpcmInfo;
            } StrmChannel;

            class StrmBufferPool {
            public:
                void Setup(void* base, u32 size, int count);
                void Shutdown();

                void* Alloc();
                void Free(void* buffer);

                u32 GetBlockSize() const { return mBlockSize; }

            private:
                static const int BLOCK_MAX = 32;
                static const int BITS_PER_BYTE = 8;

                /* 0x00 */ void* mBuffer;
                /* 0x04 */ u32 mBufferSize;
                /* 0x08 */ u32 mBlockSize;
                /* 0x0C */ int mBlockCount;
                /* 0x10 */ int mAllocCount;
                /* 0x14 */ u8 mAllocFlags[BLOCK_MAX / BITS_PER_BYTE];
            };

        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
