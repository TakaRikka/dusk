#ifndef NW4HBM_SND_SOUND_PLAYER_H
#define NW4HBM_SND_SOUND_PLAYER_H

#include <revolution/types.h>

#include "BasicSound.h"
#include "PlayerHeap.h"
#include "snd_global.h"

namespace nw4hbm {
    namespace snd {
        namespace detail {
            class ExternalSoundPlayer;
            class SeqSound;
            template <typename T>
            class SoundInstanceManager;
            class StrmSound;
            class WaveSound;
        }  // namespace detail

        class SoundPlayer {
        public:
            SoundPlayer();
            ~SoundPlayer();

            void InitParam();
            void Update();

            void StopAllSound(int frames);
            void PauseAllSound(bool flag, int frames);

            void SetVolume(f32 volume);

            int detail_GetOutputLine() const;
            bool detail_IsEnabledOutputLine() const;

            f32 detail_GetRemoteOutVolume(int remoteIndex) const;

            void detail_InsertSoundList(detail::BasicSound* sound);
            void detail_RemoveSoundList(detail::BasicSound* sound);

            void detail_InsertPriorityList(detail::BasicSound* sound);
            void detail_RemovePriorityList(detail::BasicSound* sound);

            void detail_SortPriorityList();

            detail::SeqSound*
            detail_AllocSeqSound(int priority, int startPriority,
                                 detail::BasicSound::AmbientArgInfo* ambientArgInfo,
                                 detail::ExternalSoundPlayer* extPlayer, u32 id,
                                 detail::SoundInstanceManager<detail::SeqSound>* manager);

            detail::StrmSound*
            detail_AllocStrmSound(int priority, int startPriority,
                                  detail::BasicSound::AmbientArgInfo* ambientArgInfo,
                                  detail::ExternalSoundPlayer* extPlayer, u32 id,
                                  detail::SoundInstanceManager<detail::StrmSound>* manager);

            detail::WaveSound*
            detail_AllocWaveSound(int priority, int startPriority,
                                  detail::BasicSound::AmbientArgInfo* ambientArgInfo,
                                  detail::ExternalSoundPlayer* extPlayer, u32 id,
                                  detail::SoundInstanceManager<detail::WaveSound>* manager);

            int CalcPriorityReduction(detail::BasicSound::AmbientArgInfo* ambientArgInfo, u32 id);

            void InitAmbientArg(detail::BasicSound* sound,
                                detail::BasicSound::AmbientArgInfo* ambientArgInfo);

            void SetPlayableSoundCount(int count);
            void detail_SetPlayableSoundLimit(int limit);

            bool CheckPlayableSoundCount(int startPriority, detail::ExternalSoundPlayer* extPlayer);

            void detail_AppendPlayerHeap(detail::PlayerHeap* pHeap);
            void detail_AppendPlayerHeap2(detail::PlayerHeap* pHeap);
            detail::PlayerHeap* detail_AllocPlayerHeap(detail::BasicSound* sound);
            void detail_FreePlayerHeap(detail::BasicSound* sound);

            bool detail_AppendSound(detail::BasicSound* sound);

            int GetPlayingSoundCount() const { return mSoundList.GetSize(); }
            int GetPlayableSoundCount() const { return mPlayableCount; }

            f32 GetVolume() const { return mVolume; }

            detail::BasicSound* detail_GetLowestPrioritySound() {
                //! @bug UB when the list is empty
                return &mPriorityList.GetFront();
            }

            f32 detail_GetMainOutVolume() const { return mMainOutVolume; }

        private:
            /* 0x00 */ detail::BasicSoundPlayerPlayList mSoundList;
            /* 0x0C */ detail::BasicSoundPlayerPrioList mPriorityList;
            /* 0x18 */ detail::PlayerHeapList mHeapList;
            /* 0x24 */ u16 mPlayableCount;
            /* 0x26 */ u16 mPlayableLimit;
            /* 0x28 */ f32 mVolume;
            /* 0x2C */ bool mOutputLineFlagEnable;
            /* 0x2D */ bool mUsePlayerHeap;
            /* 0x30 */ int mOutputLineFlag;
            /* 0x34 */ f32 mMainOutVolume;
            /* 0x38 */ f32 mRemoteOutVolume[WPAD_MAX_CONTROLLERS];
        };

    }  // namespace snd
}  // namespace nw4hbm

#endif
