#ifndef NW4HBM_SND_WAVE_FILE_H
#define NW4HBM_SND_WAVE_FILE_H

#include "AxVoice.h"
#include "snd_types.h"

namespace nw4hbm {
    namespace snd {
        namespace detail {
            namespace WaveFile {
                typedef enum Format { FORMAT_PCM8 = 0, FORMAT_PCM16, FORMAT_ADPCM } Format;

                typedef struct WaveInfo {
                    /* 0x00 */ u8 format;
                    /* 0x01 */ u8 loopFlag;
                    /* 0x02 */ u8 numChannels;
                    /* 0x03 */ u8 sampleRate24;
                    /* 0x04 */ u16 sampleRate;
                    /* 0x06 */ u16 padding2;
                    /* 0x08 */ u32 loopStart;
                    /* 0x0C */ u32 loopEnd;
                    /* 0x10 */ u32 channelInfoTableOffset;
                    /* 0x14 */ u32 dataOffset;
                    /* 0x18 */ u32 reserved;
                } WaveInfo;

                typedef struct WaveChannelInfo {
                    /* 0x00 */ u32 channelDataOffset;
                    /* 0x04 */ u32 adpcmOffset;
                    /* 0x08 */ u32 volumeFrontLeft;
                    /* 0x0C */ u32 volumeFrontRight;
                    /* 0x10 */ u32 volumeRearLeft;
                    /* 0x14 */ u32 volumeRearRight;
                    /* 0x18 */ u32 reserved;
                } WaveChannelInfo;
            }  // namespace WaveFile

            typedef struct ChannelParam {
                /* 0x00 */ void* dataAddr;
                /* 0x04 */ u32 volumeFrontLeft;
                /* 0x08 */ u32 volumeFrontRight;
                /* 0x0C */ u32 volumeRearLeft;
                /* 0x10 */ u32 volumeRearRight;
                /* 0x14 */ AdpcmInfo adpcmInfo;
            } ChannelParam;

            typedef struct WaveData {
                /* 0x00 */ u8 format;
                /* 0x01 */ u8 loopFlag;
                /* 0x02 */ u8 numChannels;
                /* 0x04 */ int sampleRate;
                /* 0x08 */ u32 loopStart;
                /* 0x0C */ u32 loopEnd;
                /* 0x10 */ ChannelParam channelParam[CHANNEL_MAX];
            } WaveData;

            class WaveFileReader {
            public:
                explicit WaveFileReader(const WaveFile::WaveInfo* waveInfo);

                bool ReadWaveParam(WaveData* waveData, const void* waveAddr) const;
                static AxVoice::Format WaveFormatToAxFormat(u32 format);

            private:
                /* 0x00 */ const WaveFile::WaveInfo* mWaveInfo;
            };

            inline AxVoice::Format WaveFormatToAxFormat(u32 format) {
                if (format == WaveFile::FORMAT_PCM16) {
                    return AxVoice::FORMAT_PCM16;
                }
                if (format == WaveFile::FORMAT_PCM8) {
                    return AxVoice::FORMAT_PCM8;
                }
                return AxVoice::FORMAT_ADPCM;
            }
        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
