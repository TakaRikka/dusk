#ifndef NW4HBM_SND_WAVE_PLAYER_H
#define NW4HBM_SND_WAVE_PLAYER_H

#include "snd_types.h"

#include "AxVoice.h"

#include "../ut/LinkList.h"

#include <revolution/wpad.h>

namespace nw4hbm {
    namespace snd {

        namespace detail {
            class SoundThread;
        }

        class WavePlayer {
        public:
            friend class detail::SoundThread;
            typedef struct WaveBufferInfo {
                int channelCount;
                void* bufferAddress[CHANNEL_MAX];
                u32 bufferSize;
            } WaveBufferInfo;

            class WavePacket {
                WavePacket();
                virtual ~WavePacket() {}

            private:
                /* 0x04 */ WaveBufferInfo mWaveBuffer;
                /* 0x14 */ bool mAppendFlag;

            public:
                /* 0x18 */ ut::LinkListNode mLinkNode;

                friend class WavePlayer;
            };
            typedef ut::LinkList<WavePacket, offsetof(WavePacket, mLinkNode)> WavePacketList;

            typedef enum WavePacketCallbackStatus {
                WAVE_PACKET_CALLBACK_STATUS_FINISH = 0,
                WAVE_PACKET_CALLBACK_STATUS_CANCEL,
            } WavePacketCallbackStatus;

            typedef void (*WavePacketCallback)(WavePacketCallbackStatus, WavePlayer*, WavePacket*,
                                               void*);

            typedef struct SetupParam {
                /* 0x00 */ int channelCount;
                /* 0x04 */ SampleFormat sampleFormat;
                /* 0x08 */ int sampleRate;
                /* 0x0C */ f32 pitchMax;
                /* 0x10 */ int voiceCount;
            } SetupParam;

            WavePlayer();
            virtual ~WavePlayer();

            void InitParam();
            bool Setup(const SetupParam& setupParam, WavePacketCallback callback,
                       void* callbackArg);

            bool AppendWavePacket(WavePacket* packet);

            void Start();
            void Stop();
            void Pause(bool flag);

            void Shutdown();

            void StartVoice();
            void StopVoice();

            bool IsNextWavePacket() const;
            void SetNextWavePacket();
            void UpdateWavePacket();

        private:
            void detail_Update();
            void detail_UpdateBuffer();

            static void detail_UpdateAllPlayers();
            static void detail_UpdateBufferAllPlayers();
            static void detail_StopAllPlayers();

            static void VoiceCallbackFunc(detail::AxVoice* voice,
                                          detail::AxVoice::CallbackStatus status, void* arg);

            /* 0x04 */ WavePacketList mWavePacketList;
            /* 0x10 */ int mChannelCount;
            /* 0x14 */ f32 mPitchMax;
            /* 0x18 */ detail::AxVoice* mVoice;
            /* 0x1C */ bool mStartFlag;
            /* 0x1D */ bool mVoiceStartFlag;
            /* 0x1E */ bool mLoopSetFlag;
            /* 0x1F */ bool mPauseFlag;
            /* 0x20 */ SampleFormat mSampleFormat;
            /* 0x24 */ int mSampleRate;
            /* 0x28 */ s64 mPlaySampleCount;
            /* 0x30 */ f32 mVolume;
            /* 0x34 */ f32 mPan;
            /* 0x38 */ f32 mSurroundPan;
            /* 0x3C */ f32 mPitch;
            /* 0x40 */ f32 mLpfFreq;
            /* 0x44 */ int mOutputLineFlag;
            /* 0x48 */ f32 mMainOutVolume;
            /* 0x4C */ f32 mRemoteOutVolume[WPAD_MAX_CONTROLLERS];
            /* 0x5C */ f32 mMainSend;
            /* 0x60 */ f32 mFxSend[AUX_BUS_NUM];
            /* 0x6C */ f32 mRemoteSend[WPAD_MAX_CONTROLLERS];
            /* 0x7C */ f32 mRemoteFxSend[WPAD_MAX_CONTROLLERS];
            /* 0x8C */ WavePacketCallback mCallback;
            /* 0x90 */ void* mCallbackArg;

        public:
            /* 0x94 */ ut::LinkListNode mPlayerLink;
        };

        typedef ut::LinkList<WavePlayer, offsetof(WavePlayer, mPlayerLink)> WavePlayerList;

    }  // namespace snd
}  // namespace nw4hbm

#endif
