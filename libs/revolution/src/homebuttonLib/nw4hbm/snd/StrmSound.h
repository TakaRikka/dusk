#ifndef NW4HBM_SND_STRM_SOUND_H
#define NW4HBM_SND_STRM_SOUND_H

#include <revolution/types.h>

#include "BasicSound.h"
#include "MoveValue.h"
#include "StrmPlayer.h"
#include "debug.h"

#include "../ut/LinkList.h"

namespace nw4hbm {
    namespace snd {
        class StrmSoundHandle;
        namespace detail {
            template <typename T>
            class SoundInstanceManager;
        }

        namespace detail {

            class StrmSound : public BasicSound {
                friend class StrmSoundHandle;

            public:
                explicit StrmSound(SoundInstanceManager<StrmSound>* manager);

                /* 0x08 */ NW4HBM_UT_RUNTIME_TYPEINFO;
                /* 0x28 */ virtual void Shutdown();
                /* 0x4C */ virtual void SetPlayerPriority(int priority);
                /* 0x5C */ virtual bool IsAttachedTempSpecialHandle();
                /* 0x60 */ virtual void DetachTempSpecialHandle();
                /* 0x68 */ virtual BasicPlayer& GetBasicPlayer() { return mStrmPlayer; }
                /* 0x6C */ virtual const BasicPlayer& GetBasicPlayer() const { return mStrmPlayer; }

                void Setup(const StrmPlayer::StrmHeader* header);

                bool Prepare(StrmBufferPool* bufferPool,
                             StrmPlayer::StartOffsetType startOffsetType, s32 startOffset,
                             int voices, StrmPlayer::StrmCallback* callback, u32 callbackData);

                void SetChannelPriority(int priority);

            private:
                /* 0x0D8 */ StrmPlayer mStrmPlayer;
                /* 0x1F4 */ StrmSoundHandle* mTempSpecialHandle;
                /* 0x1F8 */ SoundInstanceManager<StrmSound>* mManager;
                /* 0x5FC */ u8 mNumChannels;
                /* 0x600 */ void* mBuffer[CHANNEL_MAX];
                /* 0x608 */ u32 mBufSize;
                /* 0x60C */ u8 mNumBufBlocks;
                /* 0x610 */ int mAllocChannelCount;
            };
        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
