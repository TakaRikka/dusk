#ifndef NW4HBM_SND_PLAYER_HEAP_H
#define NW4HBM_SND_PLAYER_HEAP_H

#include "SoundHeap.h"
#include "SoundMemoryAllocatable.h"

namespace nw4hbm {
    namespace snd {

        namespace detail {
            class BasicSound;
        }

        class SoundPlayer;

    }  // namespace snd
}  // namespace nw4hbm

namespace nw4hbm {
    namespace snd {
        namespace detail {

            class PlayerHeap : public SoundHeap {
            public:
                PlayerHeap() : mSound(NULL), mPlayer(NULL) {}
                /* 0x08 */ virtual ~PlayerHeap() {}

                void SetSound(BasicSound* sound) { mSound = sound; }
                void SetSoundPlayer(SoundPlayer* player) { mPlayer = player; }

            public:
                /* 0x2C */ ut::LinkListNode mLink;

            private:
                /* 0x34 */ BasicSound* mSound;
                /* 0x38 */ SoundPlayer* mPlayer;
            };
            typedef ut::LinkList<PlayerHeap, offsetof(PlayerHeap, mLink)> PlayerHeapList;
        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
