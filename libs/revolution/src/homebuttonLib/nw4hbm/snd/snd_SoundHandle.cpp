#include "BasicSound.h"
#include "SoundHandle.h"

#include "../db/assert.h"

namespace nw4hbm {
    namespace snd {
        void SoundHandle::DetachSound() {
            if (IsAttachedSound()) {
                if (mSound->mGeneralHandle == this) {
                    mSound->mGeneralHandle = NULL;
                }

                if (mSound->mTempGeneralHandle == this) {
                    mSound->mTempGeneralHandle = NULL;
                }
            }

            if (mSound) {
                mSound = NULL;
            }
        }
    }  // namespace snd
}  // namespace nw4hbm
