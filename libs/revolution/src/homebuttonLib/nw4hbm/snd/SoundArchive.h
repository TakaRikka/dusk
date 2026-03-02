#ifndef NW4HBM_SND_SOUND_ARCHIVE_H
#define NW4HBM_SND_SOUND_ARCHIVE_H

#include <revolution/types.h>
#include "../ut/FileStream.h"
#include "snd_global.h"
#include "snd_types.h"


namespace nw4hbm {
    namespace snd {
        namespace detail {
            class SoundArchiveFileReader;
        }

        typedef enum SoundType {
            SOUND_TYPE_INVALID = 0,
            SOUND_TYPE_SEQ,
            SOUND_TYPE_STRM,
            SOUND_TYPE_WAVE
        } SoundType;

        class SoundArchive {
        public:
            typedef struct SoundInfo {
                /* 0x00 */ u32 fileId;
                /* 0x04 */ u32 playerId;
                /* 0x08 */ int playerPriority;
                /* 0x0C */ int volume;
            } SoundInfo;

            typedef struct SeqSoundInfo {
                /* 0x00 */ u32 dataOffset;
                /* 0x04 */ u32 bankId;
                /* 0x08 */ u32 allocTrack;
                /* 0x0C */ int channelPriority;
            } SeqSoundInfo;

            typedef struct StrmSoundInfo {
                /* Nothing here to see */
            } StrmSoundInfo;

            typedef struct WaveSoundInfo {
                /* 0x00 */ int subNo;
                /* 0x04 */ int channelPriority;
            } WaveSoundInfo;

            typedef struct Sound3DParam {
                /* 0x00 */ u32 flags;
                /* 0x04 */ u8 decayCurve;
                /* 0x05 */ u8 decayRatio;
            } Sound3DParam;

            typedef struct BankInfo {
                /* 0x0 */ u32 fileId;
            } BankInfo;

            typedef struct PlayerInfo {
                /* 0x00 */ int playableSoundCount;
                /* 0x04 */ u32 heapSize;
            } PlayerInfo;

            typedef struct GroupInfo {
                /* 0x00 */ u32 itemCount;
                /* 0x04 */ const char* extFilePath;
                /* 0x08 */ u32 offset;
                /* 0x0C */ u32 size;
                /* 0x10 */ u32 waveDataOffset;
                /* 0x14 */ u32 waveDataSize;
            } GroupInfo;

            typedef struct GroupItemInfo {
                /* 0x00 */ u32 fileId;
                /* 0x04 */ u32 offset;
                /* 0x08 */ u32 size;
                /* 0x0C */ u32 waveDataOffset;
                /* 0x10 */ u32 waveDataSize;
            } GroupItemInfo;

            typedef struct FileInfo {
                /* 0x00 */ u32 fileSize;
                /* 0x04 */ u32 waveDataFileSize;
                /* 0x08 */ const char* extFilePath;
                /* 0x0C */ u32 filePosCount;
            } FileInfo;

            typedef struct FilePos {
                /* 0x00 */ u32 groupId;
                /* 0x04 */ u32 index;
            } FilePos;

            typedef struct SoundArchivePlayerInfo {
                /* 0x00 */ int seqSoundCount;
                /* 0x04 */ int seqTrackCount;
                /* 0x08 */ int strmSoundCount;
                /* 0x0C */ int strmTrackCount;
                /* 0x10 */ int strmChannelCount;
                /* 0x14 */ int waveSoundCount;
                /* 0x18 */ int waveTrackCount;
            } SoundArchivePlayerInfo;

            static const u32 INVALID_ID = 0xFFFFFFFF;

        public:
            SoundArchive();
            /* 0x08 */ virtual ~SoundArchive();
            /* 0x0C */ virtual const void* detail_GetFileAddress(u32 id) const = 0;
            /* 0x10 */ virtual const void* detail_GetWaveDataFileAddress(u32 id) const = 0;
            /* 0x14 */ virtual int detail_GetRequiredStreamBufferSize() const = 0;
            /* 0x18 */ virtual ut::FileStream* OpenStream(void* buffer, int bufferSize, u32 offset,
                                                          u32 length) const = 0;
            /* 0x1C */ virtual ut::FileStream* OpenExtStream(void* buffer, int bufferSize,
                                                             const char* extPath, u32 offset,
                                                             u32 length) const = 0;

            bool IsAvailable() const;

            void Setup(detail::SoundArchiveFileReader* fileReader);
            void Shutdown();

            u32 GetPlayerCount() const;
            u32 GetGroupCount() const;

            const char* GetSoundLabelString(u32 id) const;
            u32 ConvertLabelStringToSoundId(const char* label) const;
            u32 ConvertLabelStringToPlayerId(const char* label) const;
            u32 ConvertLabelStringToGroupId(const char* label) const;

            u32 GetSoundUserParam(u32 id) const;
            SoundType GetSoundType(u32 id) const;

            bool ReadSoundInfo(u32 id, SoundInfo* info) const;
            bool detail_ReadSeqSoundInfo(u32 id, SeqSoundInfo* info) const;
            bool detail_ReadStrmSoundInfo(u32 id, StrmSoundInfo* info) const;
            bool detail_ReadWaveSoundInfo(u32 id, WaveSoundInfo* info) const;

            bool ReadPlayerInfo(u32 id, PlayerInfo* info) const;
            bool ReadSoundArchivePlayerInfo(SoundArchivePlayerInfo* info) const;

            bool detail_ReadBankInfo(u32 id, BankInfo* info) const;
            bool detail_ReadGroupInfo(u32 id, GroupInfo* info) const;
            bool detail_ReadGroupItemInfo(u32 groupId, u32 itemId, GroupItemInfo* info) const;

            bool detail_ReadFileInfo(u32 id, FileInfo* info) const;
            bool detail_ReadFilePos(u32 fileId, u32 posId, FilePos* pos) const;

            ut::FileStream* detail_OpenFileStream(u32 id, void* buffer, int bufferSize) const;
            ut::FileStream* detail_OpenGroupStream(u32 id, void* buffer, int bufferSize) const;
            ut::FileStream* detail_OpenGroupWaveDataStream(u32 id, void* buffer,
                                                           int bufferSize) const;

            void SetExternalFileRoot(const char* extFileRoot);

        protected:
            static const int FILE_PATH_MAX = 256;

        private:
            ut::FileStream* OpenExtStreamImpl(void* buffer, int bufferSize, const char* extPath,
                                              u32 offset, u32 size) const;

        private:
            /* 0x04 */ detail::SoundArchiveFileReader* mFileReader;
            /* 0x08 */ char mExtFileRoot[FILE_PATH_MAX];
        };

    }  // namespace snd
}  // namespace nw4hbm

#endif
