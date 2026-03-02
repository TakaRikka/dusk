
#include "SoundArchivePlayer.h"
#include "SoundPlayer.h"

#include "../db/assert.h"

namespace nw4hbm {
    namespace snd {
        SoundPlayer& SoundArchivePlayer::GetSoundPlayer(u32 playerId) {
            NW4R_ASSERT_MINMAX(690, playerId, 0, mSoundPlayerCount);
            return mSoundPlayers[playerId];
        }
    }
}
