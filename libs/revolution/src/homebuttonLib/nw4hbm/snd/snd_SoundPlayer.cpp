#include "SoundPlayer.h"

#include "../db/assert.h"

namespace nw4hbm {
    namespace snd {
        void SoundPlayer::StopAllSound(int frames) {
            for (detail::BasicSoundPlayerPlayList::Iterator it = mSoundList.GetBeginIter();
                 it != mSoundList.GetEndIter();)
            {
                detail::BasicSoundPlayerPlayList::Iterator currIt = it++;
                currIt->Stop(frames);
            }
        }

        void SoundPlayer::SetVolume(f32 volume) {
            NW4HBM_ASSERT(153, volume >= 0.0f);
            mVolume = volume;
        }
    }  // namespace snd
}  // namespace nw4hbm
