#ifndef NW4HBM_SND_AX_VOICE_H
#define NW4HBM_SND_AX_VOICE_H

#include <revolution/ax.h>
#include <revolution/wpad.h>
#include "DisposeCallbackManager.h"
#include <climits>
#include "snd_global.h"
#include "snd_types.h"

#include "../ut/LinkList.h"

#include "global.h"

namespace nw4hbm {
    namespace snd {
        namespace detail {

            inline int CalcAxvpbDelta(u16 init, u16 target) {
                return (target - init) / AX_SAMPLES_PER_FRAME;
            }

            inline u16 CalcMixVolume(f32 volume) {
                return ut::Min<u32>(USHRT_MAX, AX_MAX_VOLUME * volume);
            }

            class AxVoiceParamBlock {
            public:
                AxVoiceParamBlock();

                operator AXVPB*() { return mVpb; }

                bool IsAvailable() const { return mVpb != NULL; }

                bool IsRun() const { return IsAvailable() && mVpb->pb.state == AX_VOICE_RUN; }

                u32 GetCurrentAddress() const {
                    if (!IsAvailable()) {
                        return 0;
                    }

                    return (mVpb->pb.addr.currentAddressHi << 16) + mVpb->pb.addr.currentAddressLo;
                }

                u32 GetLoopAddress() const {
                    if (!IsAvailable()) {
                        return 0;
                    }

                    return (mVpb->pb.addr.loopAddressHi << 16) + mVpb->pb.addr.loopAddressLo;
                }

                u32 GetEndAddress() const {
                    if (!IsAvailable()) {
                        return 0;
                    }

                    return (mVpb->pb.addr.endAddressHi << 16) + mVpb->pb.addr.endAddressLo;
                }

                void SetVoiceAddr(const AXPBADDR& rAddr) {
                    if (IsAvailable()) {
                        // AXSetVoiceAddr doesn't actually modify the object
                        AXSetVoiceAddr(mVpb, const_cast<AXPBADDR*>(&rAddr));
                    }
                }

                void SetVoicePriority(u32 priority) {
                    if (IsAvailable()) {
                        AXSetVoicePriority(mVpb, priority);
                    }
                }

                void SetVoiceStateRun() {
                    if (IsAvailable()) {
                        AXSetVoiceState(mVpb, AX_VOICE_RUN);
                    }
                }

                void SetVoiceStateStop() {
                    if (IsRun()) {
                        AXSetVoiceState(mVpb, AX_VOICE_STOP);
                    }
                }

                void Sync();
                bool IsRmtIirEnable() const;

                void Set(AXVPB* pVpb);
                void Clear();

                void SetVoiceType(u16 type);
                void SetVoiceVe(u16 volume, u16 initVolume);
                void SetVoiceMix(const AXPBMIX& rMix, bool syncNow);
                void SetVoiceLoop(u16 loop);
                void SetVoiceLoopAddr(u32 addr);
                void SetVoiceEndAddr(u32 addr);
                void SetVoiceAdpcm(const AXPBADPCM& rAdpcm);
                void SetVoiceSrcType(u32 type);
                void SetVoiceSrc(const AXPBSRC& rSrc);
                void SetVoiceSrcRatio(f32 ratio);
                void SetVoiceAdpcmLoop(const AXPBADPCMLOOP& rLoop);
                void SetVoiceLpf(const AXPBLPF& rLpf);
                void SetVoiceLpfCoefs(u16 a0, u16 b0);
                void SetVoiceRmtOn(u16 on);
                void SetVoiceRmtMix(const AXPBRMTMIX& rMix);
                void SetVoiceRmtIIR(const AXPBRMTIIR& rIir);
                void SetVoiceRmtIIRCoefs(u16 type, ...);
                void UpdateDelta();

            private:
                static const u16 DEFAULT_VOLUME = AX_MAX_VOLUME;

            private:
                /* 0x00 */ AXVPB* mVpb;
                /* 0x04 */ u32 mSync;
                /* 0x08 */ volatile AXPBVE mPrevVeSetting;
                /* 0x0C */ bool mFirstVeUpdateFlag;
                /* 0x0E */ u16 mVolume;
            };

            class WaveData;
            class AxVoice : public DisposeCallback {
                friend class AxManager;

            public:
                typedef enum CallbackStatus {
                    CALLBACK_STATUS_FINISH_WAVE = 0,
                    CALLBACK_STATUS_INVALIDATE_WAVE,
                    CALLBACK_STATUS_DROP_VOICE,
                    CALLBACK_STATUS_DROP_DSP
                } CallbackStatus;

                typedef enum VoiceSyncFlag {
                    SYNC_AX_SRC_INITIAL = (1 << 0),
                    SYNC_AX_VOICE = (1 << 1),
                    SYNC_AX_SRC = (1 << 3),
                    SYNC_AX_VE = (1 << 4),
                    SYNC_AX_MIX = (1 << 5),
                    SYNC_AX_LPF = (1 << 6),
                    SYNC_AX_REMOTE = (1 << 7),
                    SYNC_AX_BIQUAD = (1 << 8),
                } VoiceSyncFlag;

                typedef enum Format {
                    FORMAT_PCM16 = 10,
                    FORMAT_PCM8 = 25,
                    FORMAT_ADPCM = 0
                } Format;

                typedef enum VoiceType { VOICE_TYPE_NORMAL = 0, VOICE_TYPE_STREAM } VoiceType;

                typedef enum SrcType {
                    SRC_NONE = 0,
                    SRC_LINEAR,
                    SRC_4TAP_8K,
                    SRC_4TAP_12K,
                    SRC_4TAP_16K,
                    SRC_4TAP_AUTO
                } SrcType;

                typedef void (*AxVoiceCallback)(AxVoice* drovoice, CallbackStatus status,
                                                void* callbackArg);

                AxVoice();

                /* 0x08 */ virtual ~AxVoice();
                /* 0x0C */ virtual void InvalidateData(const void* start, const void* end) {}
                /* 0x10 */ virtual void InvalidateWaveData(const void* start, const void* end);

                void Setup(const WaveData& waveParam);
                void Update();
                void Free();

                void Start();
                void Stop();

                void Pause(bool flag);

                void SetVolume(f32 volume);
                void SetVeVolume(f32 targetVolume, f32 initVolume);
                void SetMainOutVolume(f32 volume);
                void SetRemoteOutVolume(int remoteIndex, f32 volume);

                void SetPitch(f32 pitch);
                void SetPan(f32 pan);
                void SetPan2(f32 pan);
                void SetSurroundPan(f32 surroundPan);
                void SetSurroundPan2(f32 surroundPan);

                void SetLpfFreq(f32 lpfFreq);

                void SetOutputLine(int lineFlag);

                void SetMainSend(f32 send);
                void SetFxSend(AuxBus bus, f32 send);
                void SetRemoteSend(int remoteIndex, f32 send);
                void SetRemoteFxSend(int remoteIndex, f32 send);

                void SetPriority(int priority);
                int GetPriority() const { return mPriority; }

                u32 GetCurrentPlayingDspAddress() const;
                u32 GetLoopStartDspAddress() const;
                u32 GetLoopEndDspAddress() const;

                u32 GetCurrentPlayingSample() const;

                void SetAdpcmLoop(int channelIndex, Format format, const AdpcmLoopParam* param);
                void SetBaseAddress(int channelIndex, const void* baseAddress);

                bool IsPlayFinished() const;
                bool IsRun();

                void SetAxAddr(int channelIndex, bool loopFlag, Format format, const void* waveAddr,
                               u32 loopStart, u32 loopEnd);

                void SetLoopStart(int channelIndex, const void* baseAddress, u32 samples);
                void SetLoopEnd(int channelIndex, const void* baseAddress, u32 samples);
                void SetLoopFlag(bool loopFlag);

                void StopAtPoint(int channelIndex, const void* baseAddress, u32 samples);
                void SetVoiceType(VoiceType type);

                void UpdateAxSrc(bool initialUpdate);
                void SetAxSrcType(SrcType type);

                void SetAxAdpcm(int channelIndex, Format format, const AdpcmParam* param);
                void SetAxAdpcmLoop(int channelIndex, Format format,
                                    const AdpcmLoopParam* param) NO_INLINE;

                bool UpdateAxVe();
                void UpdateAxMix();
                void UpdateAxLpf();

                void InitParam(int channelCount, int voiceOutCount, AxVoiceCallback callback,
                               void* callbackData);
                bool Acquire(int channelCount, int voiceOutCount, int priority,
                             AxVoiceCallback callback, void* callbackData);

                int GetAxVoiceCount() const { return mChannelCount * mVoiceOutCount; }

                static u32 GetDspAddressBySample(const void* baseAddress, u32 samples,
                                                 Format format);
                static u32 GetSampleByDspAddress(const void* baseAddress, u32 addr, Format format);
                static u32 GetSampleByByte(u32 byte, Format format);

                template <typename T>
                void SetAxParam(void (*func)(AXVPB*, T), T param) {
                    for (int i = 0; i < mChannelCount; i++) {
                        for (int j = 0; j < mVoiceOutCount; j++) {
                            if (mVpb[i][j] != NULL) {
                                func(mVpb[i][j], param);
                            }
                        }
                    }
                }

                Format GetFormat() { return mFormat; }

                static const int VOICES_MAX = 4;
                static const int PRIORITY_MAX = 255;

            protected:
                static void VoiceCallback(void* callbackData);

                void TransformDpl2Pan(f32* outPan, f32* outSurroundPan, f32 inPan,
                                      f32 inSurroundPan);

                void CalcAXPBMIX(int channelIndex, int voiceIndex, AXPBMIX* mix);
                void CalcAXPBRMTMIX(int channelIndex, int voiceIndex, AXPBRMTMIX* mix);

                /* 0x000 (base) */
                /* 0x00C */ AXVPB* mVpb[CHANNEL_MAX][VOICES_MAX];
                /* 0x02C */ VoiceChannelParam mVoiceChannelParam[CHANNEL_MAX];
                /* 0x094 */ SoundParam mVoiceOutParam[VOICES_MAX];
                /* 0x104 */ s32 mChannelCount;
                /* 0x108 */ s32 mVoiceOutCount;
                /* 0x10C */ AxVoiceCallback mCallback;
                /* 0x110 */ void* mCallbackData;
                /* 0x114 */ int mSampleRate;
                /* 0x118 */ Format mFormat;
                /* 0x11C */ bool mActiveFlag;
                /* 0x11D */ bool mStartFlag;
                /* 0x11E */ bool mStartedFlag;
                /* 0x11F */ bool mPauseFlag;
                /* 0x120 */ bool mPausingFlag;
                /* 0x121 */ bool mFirstVeUpdateFlag;
                /* 0x122 */ bool mHomeButtonMuteFlag;
                /* 0x123 */ u8 mSyncFlag;
                /* 0x124 */ int mPriority;
                /* 0x128 */ f32 mPan;
                /* 0x12C */ f32 mSurroundPan;
                /* 0x130 */ f32 mPan2;
                /* 0x134 */ f32 mSurroundPan2;
                /* 0x138 */ f32 mLpfFreq;
                /* 0x13C */ int mOutputLineFlag;
                /* 0x140 */ f32 mMainOutVolume;
                /* 0x144 */ f32 mMainSend;
                /* 0x148 */ f32 mFxSend[AUX_BUS_NUM];
                /* 0x154 */ f32 mRemoteOutVolume[VOICES_MAX];
                /* 0x164 */ f32 mRemoteSend[VOICES_MAX];
                /* 0x174 */ f32 mRemoteFxSend[VOICES_MAX];
                /* 0x184 */ f32 mPitch;
                /* 0x188 */ f32 mVolume;
                /* 0x18C */ f32 mVolumePrev[VOICES_MAX];
                /* 0x19C */ f32 mVeInitVolume;
                /* 0x1A0 */ f32 mVeTargetVolume;
                /* 0x1A4 */ u16 mGainPrev;

            public:
                /* 0x1A8 */ ut::LinkListNode mLinkNode;
            };

            typedef ut::LinkList<AxVoice, offsetof(AxVoice, mLinkNode)> AxVoiceList;

        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
