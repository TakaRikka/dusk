#include "SoundHandle.h"
#include "SoundStartable.h"

#include "../db/assert.h"

namespace nw4hbm {
    namespace snd {
        SoundStartable::StartResult SoundStartable::detail_StartSound(
            SoundHandle* soundHandle, u32 id, detail::BasicSound::AmbientArgInfo* ambientArgInfo,
            detail::ExternalSoundPlayer* extPlayer, const StartInfo* pStartInfo) {
            StartResult result =
                detail_SetupSound(soundHandle, id, ambientArgInfo, extPlayer, false, pStartInfo);

            if (result != START_SUCCESS) {
                return result;
            }

            soundHandle->StartPrepared();
            return START_SUCCESS;
        }
    }  // namespace snd
}  // namespace nw4hbm
