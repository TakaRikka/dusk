#ifndef NW4HBM_SND_SEQ_SOUND_HANDLE_H
#define NW4HBM_SND_SEQ_SOUND_HANDLE_H

#include "snd_types.h"

#include "BasicSound.h"
#include "SeqPlayer.h"
#include "SeqSound.h"

namespace nw4hbm {
    namespace snd {
        class SeqSoundHandle : private ut::NonCopyable {
        public:
            ~SeqSoundHandle() { DetachSound(); }

            void DetachSound();

            bool IsAttachedSound() const { return mSound != NULL; }

        private:
            /* 0x00 */ detail::SeqSound* mSound;
        };

    }  // namespace snd
}  // namespace nw4hbm

#endif
