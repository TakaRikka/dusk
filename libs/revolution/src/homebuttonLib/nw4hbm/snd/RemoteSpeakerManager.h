#ifndef NW4HBM_SND_REMOTE_SPEAKER_MANAGER_H
#define NW4HBM_SND_REMOTE_SPEAKER_MANAGER_H

#include <revolution/os.h>
#include <revolution/wpad.h>
#include "RemoteSpeaker.h"


namespace nw4hbm {
    namespace snd {
        namespace detail {

            class RemoteSpeakerManager {
            public:
                static RemoteSpeakerManager& GetInstance();

                RemoteSpeaker& GetRemoteSpeaker(int idx);

                void Setup();
                void Shutdown();

            private:
                static const int SPEAKER_ALARM_HZ = 150;
                static const int SPEAKER_ALARM_PERIOD_NSEC =
                    static_cast<int>(1.0f / SPEAKER_ALARM_HZ * 1000 * 1000 * 1000);

            private:
                RemoteSpeakerManager();

                static void RemoteSpeakerAlarmProc(OSAlarm* alarm, OSContext* context);

            private:
                /* 0x00 */ bool mInitialized;
                /* 0x08 */ OSAlarm mRemoteSpeakerAlarm;
                /* 0x38 */ RemoteSpeaker mSpeaker[WPAD_MAX_CONTROLLERS];
            };

        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
