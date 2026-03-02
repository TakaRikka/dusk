#ifndef NW4HBM_SND_STRM_FILE_H
#define NW4HBM_SND_STRM_FILE_H

#include <revolution/os.h>
#include <revolution/types.h>

#include <revolution/hbm.h>

#include "Util.h"
#include "snd_global.h"

#include "../ut/FileStream.h"
#include "../ut/binaryFileFormat.h"

namespace nw4hbm {
    namespace snd {
        namespace detail {
            namespace StrmFile {

                static const u32 SIGNATURE_FILE = 'RSTM';
                static const u32 SIGNATURE_HEAD_BLOCK = 'HEAD';
                static const int FILE_VERSION = NW4HBM_VERSION(1, 0);

                typedef struct StrmDataInfo {
                    /* 0x00 */ u8 format;
                    /* 0x01 */ u8 loopFlag;
                    /* 0x02 */ u8 numChannels;
                    /* 0x03 */ u8 sampleRate24;
                    /* 0x04 */ u16 sampleRate;
                    /* 0x06 */ u16 blockHeaderOffset;
                    /* 0x08 */ u32 loopStart;
                    /* 0x0C */ u32 loopEnd;
                    /* 0x10 */ u32 dataOffset;
                    /* 0x14 */ u32 numBlocks;
                    /* 0x18 */ u32 blockSize;
                    /* 0x1C */ u32 blockSamples;
                    /* 0x20 */ u32 lastBlockSize;
                    /* 0x24 */ u32 lastBlockSamples;
                    /* 0x28 */ u32 lastBlockPaddedSize;
                    /* 0x2C */ u32 adpcmDataInterval;
                    /* 0x30 */ u32 adpcmDataSize;
                } StrmDataInfo;

                typedef struct TrackInfo {
                    /* 0x00 */ u8 channelCount;
                    /* 0x01 */ u8 channelIndexTable[];
                } TrackInfo;

                typedef struct TrackTable {
                    /* 0x00 */ u8 trackCount;
                    /* 0x01 */ u8 trackDataType;
                    /* 0x04 */ Util::DataRef<TrackInfo> refTrackHeader[];
                } TrackTable;

                typedef struct ChannelInfo {
                    /* 0x00 */ Util::DataRef<AdpcmInfo> refAdpcmInfo;
                } ChannelInfo;

                typedef struct ChannelTable {
                    /* 0x00 */ u8 channelCount;
                    /* 0x04 */ Util::DataRef<ChannelInfo> refChannelHeader[];
                } ChannelTable;

                typedef struct Header {
                    /* 0x00 */ ut::BinaryFileHeader fileHeader;
                    /* 0x10 */ u32 headBlockOffset;
                    /* 0x14 */ u32 headBlockSize;
                    /* 0x18 */ u32 adpcBlockOffset;
                    /* 0x1C */ u32 adpcBlockSize;
                    /* 0x20 */ u32 dataBlockOffset;
                    /* 0x24 */ u32 dataBlockSize;
                } Header;

                typedef struct HeadBlock {
                    /* 0x00 */ ut::BinaryBlockHeader blockHeader;
                    /* 0x08 */ Util::DataRef<StrmDataInfo> refDataHeader;
                    /* 0x10 */ Util::DataRef<TrackTable> refTrackTable;
                    /* 0x18 */ Util::DataRef<ChannelTable> refChannelTable;
                } HeadBlock;
            }  // namespace StrmFile

            typedef struct StrmInfo {
                /* 0x00 */ u8 format;
                /* 0x01 */ u8 loopFlag;
                /* 0x02 */ u8 numChannels;
                /* 0x04 */ int sampleRate;
                /* 0x08 */ u16 blockHeaderOffset;
                /* 0x0C */ u32 loopStart;
                /* 0x10 */ u32 loopEnd;
                /* 0x14 */ u32 dataOffset;
                /* 0x18 */ u32 numBlocks;
                /* 0x1C */ u32 blockSize;
                /* 0x20 */ u32 blockSamples;
                /* 0x24 */ u32 lastBlockSize;
                /* 0x28 */ u32 lastBlockSamples;
                /* 0x2C */ u32 lastBlockPaddedSize;
                /* 0x30 */ u32 adpcmDataInterval;
                /* 0x34 */ u32 adpcmDataSize;
            } StrmInfo;

            class StrmFileReader {
            public:
                StrmFileReader();

                bool IsAvailable() const { return mHeader != NULL; }

                bool IsValidFileHeader(const void* buffer);
                void Setup(const void* buffer);

                bool ReadStrmInfo(StrmInfo* strmInfo) const;
                bool ReadAdpcmInfo(AdpcmInfo* adpcmInfo, int channels) const;

                u32 GetAdpcBlockOffset() const {
                    if (IsAvailable()) {
                        return mHeader->adpcBlockOffset;
                    }
                    return 0;
                }

            private:
                /* 0x00 */ const StrmFile::Header* mHeader;
                /* 0x04 */ const StrmFile::HeadBlock* mHeadBlock;
            };

            class StrmFileLoader {
            public:
                explicit StrmFileLoader(ut::FileStream& rFileStream) : mStream(rFileStream) {}

                bool LoadFileHeader(void* buffer, u32 size);
                bool ReadAdpcBlockData(u16* yn1, u16* yn2, int block, int channels);

                bool ReadStrmInfo(StrmInfo* strmInfo) const {
                    if (!mReader.IsAvailable()) {
                        return false;
                    }
                    return mReader.ReadStrmInfo(strmInfo);
                }

                bool ReadAdpcmInfo(AdpcmInfo* adpcmInfo, int channel) const {
                    if (!mReader.IsAvailable()) {
                        return false;
                    }
                    return mReader.ReadAdpcmInfo(adpcmInfo, channel);
                }

            private:
                static const int HEADER_ALIGNED_SIZE = OSRoundUp32B(sizeof(StrmFile::Header));

                /* 0x00 */ ut::FileStream& mStream;
                /* 0x04 */ StrmFileReader mReader;
            };
        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
