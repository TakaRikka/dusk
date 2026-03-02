#ifndef NW4HBM_SND_REMOTE_SPEAKER_H
#define NW4HBM_SND_REMOTE_SPEAKER_H

#include <revolution/os.h>
#include <revolution/wenc.h>
#include <revolution/wpad.h>

namespace nw4hbm {
    namespace snd {

        class RemoteSpeaker {
        public:
            static const int SAMPLES_PER_AUDIO_PACKET = 40;

            RemoteSpeaker();

            void InitParam();
            void ClearParam();

            bool Setup(WPADCallback pCallback);
            void Update(const s16* axRemoteSamples);

            bool IsAvailable() const { return mRemoteInitFlag; }
            void SetChannelIndex(int index) { mChannelIndex = index; }

        private:
            typedef enum SpeakerState {
                STATE_INVALID,
                STATE_EXEC_SPEAKER_ON,
                STATE_SPEAKER_ON,
                STATE_EXEC_SPEAKER_PLAY,
                STATE_SPEAKER_PLAY,
                STATE_EXEC_SPEAKER_OFF,
                STATE_SPEAKER_OFF
            } SpeakerState;

            typedef enum SpeakerCommand {
                COMMAND_NONE,
                COMMAND_SPEAKER_ON,
                COMMAND_SPEAKER_PLAY,
                COMMAND_SPEAKER_OFF
            } SpeakerCommand;

            bool IsAllSampleZero(const s16* axRemoteSamples);

            static const int SAMPLES_PER_ENCODED_PACKET = (SAMPLES_PER_AUDIO_PACKET + 1) / 2;
            static const int CONTINUOUS_PLAY_INTERVAL_MINUTES = 8;

            static void ContinueAlarmHandler(OSAlarm* alarm, OSContext* context);
            static void IntervalAlarmHandler(OSAlarm* alarm, OSContext* context);

            /* 0x00 */ bool mInitFlag;
            /* 0x01 */ bool mRemoteInitFlag;
            /* 0x02 */ bool mPlayFlag;
            /* 0x03 */ bool mEnableFlag;
            /* 0x04 */ bool mSetupBusyFlag;
            /* 0x05 */ bool mFirstEncodeFlag;
            /* 0x06 */ bool mForceResumeFlag;
            /* 0x07 */ bool mContinueFlag;
            /* 0x08 */ bool mIntervalFlag;
            /* 0x09 */ WENCInfo mEncodeInfo;
            /* 0x2C */ int mChannelIndex;
            /* 0x30 */ WPADCallback mSetupCallback;
            /* 0x38 */ OSAlarm mContinueAlarm;
            /* 0x68 */ OSAlarm mInvervalAlarm;
            /* 0x98 */ OSTime mContinueBeginTime;
        };

    }  // namespace snd
}  // namespace nw4hbm

#endif
