#ifndef NW4HBM_SND_EXTERNAL_SOUND_PLAYER_H
#define NW4HBM_SND_EXTERNAL_SOUND_PLAYER_H

#include <revolution/types.h>

#include "BasicSound.h"

namespace nw4hbm {
    namespace snd {
        namespace detail {
            class ExternalSoundPlayer {
            public:
                ExternalSoundPlayer();
                ~ExternalSoundPlayer();

                int GetPlayableSoundCount() const { return mPlayableCount; }
                void SetPlayableSoundCount(int count);

                int GetPlayingSoundCount() const { return mSoundList.GetSize(); }

                f32 detail_GetVolume() const { return mVolume; }
                BasicSound* GetLowestPrioritySound();

                void InsertSoundList(BasicSound* sound);
                void RemoveSoundList(BasicSound* sound);

                template <typename TForEachFunc>
                TForEachFunc ForEachSound(TForEachFunc func, bool reverse) {
                    if (reverse) {
                        BasicSoundExtPlayList::RevIterator it = mSoundList.GetBeginReverseIter();

                        while (it != mSoundList.GetEndReverseIter()) {
                            BasicSoundExtPlayList::RevIterator curr = it;

                            SoundHandle handle;
                            handle.detail_AttachSoundAsTempHandle(&*curr);
                            func(handle);

                            if (handle.IsAttachedSound()) {
                                it++;
                            }
                        }
                    } else {
                        for (BasicSoundExtPlayList::Iterator itr = mSoundList.GetBeginIter();
                             itr != mSoundList.GetEndIter();)
                        {
                            BasicSoundExtPlayList::Iterator curr = itr++;
                            SoundHandle handle;
                            handle.detail_AttachSoundAsTempHandle(&*curr);
                            func(handle);
                        }
                    }

                    return func;
                }

            private:
                /* 0x00 */ BasicSoundExtPlayList mSoundList;
                /* 0x0C */ u16 mPlayableCount;
                /* 0x10 */ f32 mVolume;
            };
        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
