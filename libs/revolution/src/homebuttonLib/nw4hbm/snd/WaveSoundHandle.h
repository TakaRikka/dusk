#ifndef NW4HBM_SND_WAVE_SOUND_HANDLE_H
#define NW4HBM_SND_WAVE_SOUND_HANDLE_H

#include "WaveSound.h"
#include "snd_types.h"

namespace nw4hbm {
    namespace snd {

        class WaveSoundHandle : private ut::NonCopyable {
        public:
            ~WaveSoundHandle() { DetachSound(); }

            void DetachSound();

            bool IsAttachedSound() const { return mSound != NULL; }

        private:
            /* 0x00 */ detail::WaveSound* mSound;
        };

    }  // namespace snd
}  // namespace nw4hbm

#endif
