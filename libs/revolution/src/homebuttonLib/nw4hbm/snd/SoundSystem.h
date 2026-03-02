#ifndef NW4HBM_SND_SOUND_SYSTEM_H
#define NW4HBM_SND_SOUND_SYSTEM_H

#include <revolution/types.h>

#include "AxManager.h"
#include "RemoteSpeakerManager.h"
#include "TaskThread.h"

namespace nw4hbm {
    namespace snd {

        class FxBase;
        class SoundSystem {
        public:
            static const int DEFAULT_DVD_THREAD_PRIORITY = 3;
            static const int DEFAULT_SOUND_THREAD_PRIORITY = 4;

            static void InitSoundSystem(s32 soundThreadPrio = DEFAULT_SOUND_THREAD_PRIORITY,
                                        s32 dvdThreadPriority = DEFAULT_DVD_THREAD_PRIORITY);

            static void ShutdownSoundSystem();

            static void SetOutputMode(OutputMode mode) {
                detail::AxManager::GetInstance().SetOutputMode(mode);
            }

            static f32 GetMasterVolume() {
                return detail::AxManager::GetInstance().GetMasterVolume();
            }
            static void SetMasterVolume(f32 volume, int frame) {
                detail::AxManager::GetInstance().SetMasterVolume(volume, frame);
            }

            static RemoteSpeaker& GetRemoteSpeaker(int idx) {
                return detail::RemoteSpeakerManager::GetInstance().GetRemoteSpeaker(idx);
            }

            static void AppendEffect(AuxBus bus, FxBase* pFx) {
                detail::AxManager::GetInstance().AppendEffect(bus, pFx);
            }
            static void ClearEffect(AuxBus bus, int frame) {
                detail::AxManager::GetInstance().ClearEffect(bus, frame);
            }
        };

    }  // namespace snd
}  // namespace nw4hbm

#endif
