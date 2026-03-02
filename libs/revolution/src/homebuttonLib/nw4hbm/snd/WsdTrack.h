#ifndef NW4HBM_SND_WSD_TRACK_H
#define NW4HBM_SND_WSD_TRACK_H

#include "snd_types.h"

#include "Channel.h"
#include "Lfo.h"
#include "WsdFile.h"

namespace nw4hbm {
    namespace snd {
        namespace detail {
            class WsdPlayer;
            class WsdTrack {
            public:
                class WsdCallback {
                public:
                    /* 0x08 */ virtual ~WsdCallback() = 0 {}

                    /* 0x0C */ virtual bool GetWaveSoundData(WaveSoundInfo* info,
                                                             WaveSoundNoteInfo* noteInfo,
                                                             WaveData* waveData,
                                                             const void* waveSoundData, int index,
                                                             int noteIndex, u32 userData) const = 0;
                };

                typedef enum StartOffsetType {
                    START_OFFSET_TYPE_SAMPLE = 0,
                    START_OFFSET_TYPE_MILLISEC
                } StartOffsetType;

                WsdTrack() : mWsdPlayer(NULL) {}

                void Init(WsdPlayer* player);

                void Start(const void* wsdData, int index);
                void Close();

                void UpdateChannel();
                void ReleaseAllChannel(int release) NO_INLINE;
                void PauseAllChannel(bool flag);
                void FreeAllChannel();
                void AddChannel(Channel* channel);

                int Parse(const WsdCallback* callback, u32 callbackData, bool doNoteOn);
                int ParseNextTick(const WsdCallback* callback, u32 callbackData, bool doNoteOn);

                static void ChannelCallbackFunc(Channel* dropChannel,
                                                Channel::ChannelCallbackStatus status,
                                                u32 userData);

                const void* GetWsdDataAddress() const { return mWsdData; }

            private:
                /* 0x00 */ const void* mWsdData;
                /* 0x04 */ int mIndex;
                /* 0x08 */ u32 mCounter;
                /* 0x0C */ LfoParam mLfoParam;
                /* 0x1C */ u8 mBendRange;
                /* 0x1D */ u8 mPriority;
                /* 0x20 */ WaveSoundInfo mWaveSoundInfo;
                /* 0x2C */ WsdPlayer* mWsdPlayer;
                /* 0x30 */ Channel* mChannelList;
            };
        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
