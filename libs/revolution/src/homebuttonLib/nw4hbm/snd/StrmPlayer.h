#ifndef NW4HBM_SND_STRM_PLAYER_H
#define NW4HBM_SND_STRM_PLAYER_H

#include "snd_types.h"

#include "AxVoice.h"

//! TODO: find a way to remove this hack
#define MAKE_DTOR_ZERO
#include "BasicPlayer.h"
#undef MAKE_DTOR_ZERO

#include "InstancePool.h"
#include "StrmChannel.h"
#include "StrmFile.h"

namespace nw4hbm {
    namespace snd {
        namespace detail {

            class StrmPlayer : public BasicPlayer {
            public:
                typedef enum StartOffsetType {
                    START_OFFSET_TYPE_SAMPLE = 0,
                    START_OFFSET_TYPE_MILLISEC
                } StartOffsetType;

                typedef struct StrmHeader {
                    //! TODO: Why 8 if the player only supports 2???
                    static const int STRM_CHANNEL_MAX = 8;

                    /* 0x00 */ StrmInfo strmInfo;
                    /* 0x38 */ AdpcmInfo adpcmInfo[STRM_CHANNEL_MAX];
                } StrmHeader;

                //! TODO: How is this calculated?
                static const int LOAD_BUFFER_SIZE = 0x4000 + 32;
                class LoadCommand {
                    friend class StrmPlayer;

                public:
                    virtual void NotifyAsyncEnd(bool result);

                    void SetAdpcmLoopContext(int channelNum, u16* predScale);
                    void* GetBuffer(int channelNum);

                private:
                    typedef enum Status {
                        STATE_SETUP = 0,
                        STATE_INTERVAL,
                    } Status;

                    /* 0x04 */ StrmPlayer* mPlayer;
                    /* 0x08 */ Status mStatus;
                    /* 0x0C */ s32 mStreamBlockIndex;
                    /* 0x10 */ s32 mBufferBlockIndex;

                public:
                    /* 0x14 */ ut::LinkListNode mLinkNode;

                    static u8 mMramBuf[LOAD_BUFFER_SIZE] ATTRIBUTE_ALIGN(32);
                };
                typedef ut::LinkList<LoadCommand, offsetof(LoadCommand, mLinkNode)> LoadCommandList;

                typedef void (*NotifyLoadHeaderAsyncEndCallback)(bool, const StrmHeader*, void*);
                class StrmCallback {
                public:
                    typedef enum Result {
                        RESULT_SUCCESS = 0,
                        RESULT_FAILED,
                        RESULT_CANCELED,
                        RESULT_ASYNC,
                        RESULT_RETRY
                    } Result;

                    virtual ~StrmCallback() {}

                    virtual Result LoadHeader(StrmPlayer::NotifyLoadHeaderAsyncEndCallback callback,
                                              void* callbackData, u32 userId,
                                              u32 userData) const = 0;
                    virtual Result LoadStream(void* mramAddr, u32 size, s32 offset, int numChannels,
                                              u32 blockSize, s32 blockHeaderOffset,
                                              bool needUpdateAdpcmLoop,
                                              StrmPlayer::LoadCommand& callback, u32 userId,
                                              u32 userData) const = 0;

                    virtual void CancelLoading(u32 userId, u32 userData) const = 0;
                };

            public:
                StrmPlayer();

                /* 0x0C */ virtual bool Start();
                /* 0x10 */ virtual void Stop();
                /* 0x14 */ virtual void Pause(bool flag);
                /* 0x18 */ virtual bool IsActive() const { return mActiveFlag; }
                /* 0x1C */ virtual bool IsPrepared() const { return mPreparedFlag; };
                /* 0x20 */ virtual bool IsStarted() const { return mStartedFlag; };
                /* 0x24 */ virtual bool IsPause() const { return mPauseFlag; };
                /* 0x28 */ virtual void SetVolume(f32 volume) { mExtVolume = volume; }
                /* 0x2C */ virtual void SetPitch(f32 pitch) { mExtPitch = pitch; }
                /* 0x30 */ virtual void SetPan(f32 pan) { mExtPan = pan; }
                /* 0x34 */ virtual void SetSurroundPan(f32 surroundPan) {
                    mExtSurroundPan = surroundPan;
                }
                /* 0x38 */ virtual void SetPan2(f32 pan2) { mExtPan2 = pan2; }
                /* 0x3C */ virtual void SetSurroundPan2(f32 surroundPan2) {
                    mExtSurroundPan2 = surroundPan2;
                }
                /* 0x40 */ virtual void SetLpfFreq(f32 lpfFreq) { mExtLpfFreq = lpfFreq; }
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

                void Setup(const StrmHeader* header);
                bool SetupPlayer(const StrmHeader* header);
                void Shutdown();

                bool Prepare(StrmBufferPool* bufferPool, StartOffsetType startOffsetType,
                             s32 startOffset, int voices, StrmCallback* callback, u32 callbackData);

                void InitParam(int voices);
                void ForceStop();

                void Update();

                static void UpdateAllPlayers();
                static void StopAllPlayers();
                static void UpdateBufferAllPlayers();

            private:
                static const int DATA_BLOCK_COUNT_MIN = 4;
                static const int DATA_BLOCK_COUNT_MAX = 32;
                static const int DATA_BLOCK_SIZE_MAX = 0x2000;

                bool AllocChannels(int channels, int voices);
                void FreeChannels();

                void UpdateBuffer();
                void UpdateLoopAddress(u32 startSample, u32 endSample);
                void UpdatePlayingBlockIndex();
                void UpdateDataLoopAddress(s32 endBlock);
                void SetLoopEndToZeroBuffer(int endBlock);
                void UpdateLoadingBlockIndex(LoadCommand::Status status);
                void UpdatePauseStatus();

                int CalcLoadingBufferBlockCount() const;
                bool CalcStartOffset(s32* blockIndex, u32* blockOffset, s32* loopCount);

                static void VoiceCallbackFunc(AxVoice* voice, AxVoice::CallbackStatus status,
                                              void* pCallbackArg);

                static void NotifyStrmHeaderAsyncEndCallback(bool result, const StrmHeader* header,
                                                             void* userData);

            public:
                /* 0x008 */ ut::LinkListNode mPlayerLink;

            private:
                /* 0x010 */ StrmInfo mStrmInfo;
                /* 0x048 */ u8 mActiveFlag;
                /* 0x049 */ u8 mStartedFlag;
                /* 0x04A */ u8 mPreparedFlag;
                /* 0x04B */ u8 mPauseFlag;
                /* 0x04C */ u8 mDiskErrorFlag;
                /* 0x04D */ u8 mPauseStatus;
                /* 0x04E */ u8 mLoadWaitFlag;
                /* 0x04F */ u8 mNoRealtimeLoadFlag;
                /* 0x050 */ u8 mSkipUpdateAdpcmLoop;
                /* 0x051 */ u8 mValidAdpcmLoop;
                /* 0x052 */ u8 mPlayFinishFlag;
                /* 0x053 */ u8 mLoadFinishFlag;
                /* 0x054 */ s32 mLoopCounter;
                /* 0x058 */ int mPrepareCounter;
                /* 0x05C */ int mChangeNumBlocks;
                /* 0x060 */ int mDataBlockSize;
                /* 0x064 */ int mBufferBlockCount;
                /* 0x068 */ int mBufferBlockCountBase;
                /* 0x06C */ int mLoadingBufferBlockCount;
                /* 0x070 */ int mLoadingBufferBlockIndex;
                /* 0x074 */ int mLoadingDataBlockIndex;
                /* 0x078 */ int mPlayingBufferBlockCount;
                /* 0x07C */ int mPlayingBufferBlockIndex;
                /* 0x080 */ int mPlayingDataBlockIndex;
                /* 0x084 */ int mLoopStartBlockIndex;
                /* 0x088 */ int mLastBlockIndex;
                /* 0x08C */ StartOffsetType mStartOffsetType;
                /* 0x090 */ int mStartOffset;
                /* 0x094 */ LoadCommandList mFreeLoadCommandList;
                /* 0x0A0 */ LoadCommandList mFillBufferCommandList;
                /* 0x0AC */ LoadCommand mLoadCoammndArray[32];
                /* 0x42C */ StrmBufferPool* mBufferPool;
                /* 0x430 */ StrmCallback* mCallback;
                /* 0x434 */ u32 mCallbackData;
                /* 0x438 */ AxVoice* mVoice;
                /* 0x43C */ f32 mExtVolume;
                /* 0x440 */ f32 mExtPan;
                /* 0x444 */ f32 mExtSurroundPan;
                /* 0x448 */ f32 mExtPan2;
                /* 0x44C */ f32 mExtSurroundPan2;
                /* 0x450 */ f32 mExtPitch;
                /* 0x454 */ f32 mExtLpfFreq;
                /* 0x458 */ int mOutputLineFlag;
                /* 0x45C */ f32 mMainOutVolume;
                /* 0x460 */ f32 mMainSend;
                /* 0x464 */ f32 mFxSend[AUX_BUS_NUM];
                /* 0x470 */ f32 mRemoteOutVolume[WPAD_MAX_CONTROLLERS];
                /* 0x480 */ f32 mRemoteSend[WPAD_MAX_CONTROLLERS];
                /* 0x490 */ f32 mRemoteFxSend[WPAD_MAX_CONTROLLERS];
                /* 0x4A0 */ s32 mChannelCount;
                /* 0x4A4 */ s32 mVoiceOutCount;
                /* 0x4A8 */ StrmChannel mChannels[CHANNEL_MAX];
                /* 0x518 */ u16 mAdpcmPredScale[CHANNEL_MAX];
            };

            typedef ut::LinkList<StrmPlayer, offsetof(StrmPlayer, mPlayerLink)> StrmPlayerList;

        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
