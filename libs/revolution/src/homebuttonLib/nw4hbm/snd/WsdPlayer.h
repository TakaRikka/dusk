#ifndef NW4HBM_SND_WSD_PLAYER_H
#define NW4HBM_SND_WSD_PLAYER_H

#include <revolution/types.h>

#include "BasicPlayer.h"
#include "WsdTrack.h"

namespace nw4hbm {
    namespace snd {
        namespace detail {

            class WsdPlayer : public BasicPlayer, public DisposeCallback {
            public:
                WsdPlayer();

                void InitParam(int voices, const WsdTrack::WsdCallback* callback, u32 callbackData);

                bool Prepare(const void* waveSoundBase, int index, int voices,
                             const WsdTrack::WsdCallback* callback, u32 callbackData);

                /* 0x0C */ virtual bool Start();
                /* 0x10 */ virtual void Stop();
                /* 0x14 */ virtual void Pause(bool flag);
                /* 0x18 */ virtual bool IsActive() const { return mActiveFlag; }
                /* 0x20 */ virtual bool IsStarted() const { return mStartedFlag; };
                /* 0x1C */ virtual bool IsPrepared() const { return mPreparedFlag; };
                /* 0x24 */ virtual bool IsPause() const { return mPauseFlag; };
                /* 0x28 */ virtual void SetVolume(f32 volume);
                /* 0x2C */ virtual void SetPitch(f32 pitch);
                /* 0x30 */ virtual void SetPan(f32 pan);
                /* 0x34 */ virtual void SetSurroundPan(f32 surroundPan);
                /* 0x38 */ virtual void SetPan2(f32 pan2);
                /* 0x3C */ virtual void SetSurroundPan2(f32 surroundPan2);
                /* 0x40 */ virtual void SetLpfFreq(f32 lpfFreq);
                /* 0x44 */ virtual f32 GetVolume() const { return mExtVolume; };
                /* 0x48 */ virtual f32 GetPitch() const { return mExtPitch; };
                /* 0x4C */ virtual f32 GetPan() const { return mExtPan; };
                /* 0x50 */ virtual f32 GetSurroundPan() const { return mExtSurroundPan; };
                /* 0x54 */ virtual f32 GetPan2() const { return mExtPan2; };
                /* 0x58 */ virtual f32 GetSurroundPan2() const { return mExtSurroundPan2; };
                /* 0x5C */ virtual f32 GetLpfFreq() const { return mExtLpfFreq; };
                /* 0x60 */ virtual void SetOutputLine(int lineFlag);
                /* 0x64 */ virtual void SetMainOutVolume(f32 volume);
                /* 0x68 */ virtual void SetMainSend(f32 send);
                /* 0x6C */ virtual void SetFxSend(AuxBus bus, f32 send);
                /* 0x70 */ virtual void SetRemoteOutVolume(int remoteIndex, f32 volume);
                /* 0x74 */ virtual void SetRemoteSend(int remoteIndex, f32 send);
                /* 0x78 */ virtual void SetRemoteFxSend(int remoteIndex, f32 send);
                /* 0x7C */ virtual int GetOutputLine() const;
                /* 0x80 */ virtual f32 GetMainOutVolume() const;
                /* 0x84 */ virtual f32 GetMainSend() const;
                /* 0x88 */ virtual f32 GetFxSend(AuxBus bus) const;
                /* 0x8C */ virtual f32 GetRemoteOutVolume(int remoteIndex) const;
                /* 0x90 */ virtual f32 GetRemoteSend(int remoteIndex) const;
                /* 0x94 */ virtual f32 GetRemoteFxSend(int remoteIndex) const;

                void SetChannelPriority(int prio);
                u8 GetChannelPriority() { return mPriority; }

                f32 GetPanRange() { return mPanRange; }

                int GetVoiceOutCount() { return mVoiceOutCount; }

                void FinishPlayer();

                BOOL ParseNextTick(bool doNoteOn);

                void UpdateChannel();
                void Update();

                static void UpdateAllPlayers();
                static void StopAllPlayers();

                virtual void InvalidateData(const void* start, const void* end);
                virtual void InvalidateWaveData(const void* start, const void* end) {}

                /* 0x14 */ ut::LinkListNode mPlayerLink;

            private:
                /* 0x1C */ bool mHomeButtonMenuFlag;
                /* 0x1D */ u8 mActiveFlag;
                /* 0x1E */ u8 mPreparedFlag;
                /* 0x1F */ u8 mStartedFlag;
                /* 0x20 */ u8 mPauseFlag;
                /* 0x21 */ u8 mSkipFlag;
                /* 0x24 */ f32 mExtVolume;
                /* 0x28 */ f32 mExtPan;
                /* 0x2C */ f32 mExtSurroundPan;
                /* 0x30 */ f32 mPanRange;
                /* 0x34 */ f32 mExtPan2;
                /* 0x38 */ f32 mExtSurroundPan2;
                /* 0x3C */ f32 mExtPitch;
                /* 0x40 */ f32 mExtLpfFreq;
                /* 0x44 */ int mOutputLineFlag;
                /* 0x48 */ f32 mMainOutVolume;
                /* 0x4C */ f32 mMainSend;
                /* 0x50 */ f32 mFxSend[AUX_BUS_NUM];
                /* 0x5C */ f32 mRemoteOutVolume[WPAD_MAX_CONTROLLERS];
                /* 0x6C */ f32 mRemoteSend[WPAD_MAX_CONTROLLERS];
                /* 0x7C */ f32 mRemoteFxSend[WPAD_MAX_CONTROLLERS];
                /* 0x8C */ int mVoiceOutCount;
                /* 0x90 */ u8 mPriority;
                /* 0x94 */ const WsdTrack::WsdCallback* mCallback;
                /* 0x98 */ u32 mCallbackData;
                /* 0x9C */ WsdTrack mTrack;
                /* 0xD0 */ u32 mTickCounter;
            };

            typedef ut::LinkList<WsdPlayer, offsetof(WsdPlayer, mPlayerLink)> WsdPlayerList;

        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
