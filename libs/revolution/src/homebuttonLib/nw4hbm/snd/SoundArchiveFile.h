#ifndef NW4HBM_SND_SOUND_ARCHIVE_FILE_H
#define NW4HBM_SND_SOUND_ARCHIVE_FILE_H

#include <revolution/types.h>

#include "SoundArchive.h"
#include "Util.h"

#include "../ut/binaryFileFormat.h"
#include "../ut/inlines.h"

#include <revolution/hbm.h>

namespace nw4hbm {
    namespace snd {
        namespace detail {
            namespace SoundArchiveFile {

                static const u32 SIGNATURE_FILE = 'RSAR';
                static const u32 SIGNATURE_INFO_BLOCK = 'INFO';
                static const u32 SIGNATURE_SYMB_BLOCK = 'SYMB';
                static const int FILE_VERSION = NW4HBM_VERSION(1, 1);

                typedef struct StringTreeNode {
                    /* 0x00 */ u16 flags;
                    /* 0x02 */ u16 bit;
                    /* 0x04 */ u32 leftIdx;
                    /* 0x08 */ u32 rightIdx;
                    /* 0x0C */ u32 strIdx;
                    /* 0x10 */ u32 id;
                } StringTreeNode;

                typedef struct StringTree {
                    /* 0x00 */ u32 rootIdx;
                    /* 0x04 */ Util::Table<StringTreeNode> nodeTable;
                } StringTree;

                typedef struct StringTable {
                    /* 0x00 */ Util::Table<u32> offsetTable;
                } StringTable;

                typedef struct StringChunk {
                    /* 0x00 */ u32 tableOffset;
                    /* 0x04 */ u32 soundTreeOffset;
                    /* 0x08 */ u32 playerTreeOffset;
                    /* 0x0C */ u32 groupTreeOffset;
                    /* 0x10 */ u32 bankTreeOffset;
                } StringChunk;

                typedef struct StringBlock {
                    union {
                        StringTable stringTable;
                        StringChunk stringChunk;
                    /* 0x00 */ };
                } StringBlock;

                typedef struct SymbolBlock {
                    /* 0x00 */ ut::BinaryBlockHeader blockHeader;
                    /* 0x08 */ StringBlock stringBlock;
                } SymbolBlock;

                typedef struct SeqSoundInfo {
                    /* 0x00 */ u32 dataOffset;
                    /* 0x04 */ u32 bankId;
                    /* 0x08 */ u32 allocTrack;
                    /* 0x0C */ u8 channelPriority;
                    /* 0x0D */ u8 releasePriorityFix;
                } SeqSoundInfo;

                typedef struct StrmSoundInfo {
                    /* Nothing to see here. */
                } StrmSoundInfo;

                typedef struct WaveSoundInfo {
                    /* 0x00 */ s32 subNo;
                    /* 0x04 */ u32 allocTrack;
                    /* 0x08 */ u8 channelPriority;
                    /* 0x09 */ u8 releasePriorityFix;
                } WaveSoundInfo;

                typedef Util::DataRef<void, SeqSoundInfo, StrmSoundInfo, WaveSoundInfo>
                    SoundInfoOffset;

                typedef struct Sound3DParam {
                    /* 0x00 */ u32 flags;
                    /* 0x04 */ u8 decayCurve;
                    /* 0x05 */ u8 decayRatio;
                } Sound3DParam;

                typedef struct SoundCommonInfo {
                    /* 0x00 */ u32 stringId;
                    /* 0x04 */ u32 fileId;
                    /* 0x08 */ u32 playerId;

                    /* 0x0C */ Util::DataRef<Sound3DParam> param3dRef;
                    /* 0x14 */ u8 volume;
                    /* 0x15 */ u8 playerPriority;
                    /* 0x16 */ u8 soundType;
                    /* 0x17 */ u8 remoteFilter;
                    /* 0x18 */ SoundInfoOffset soundInfoRef;

                    /* 0x20 */ u32 userParam[2];

                    /* 0x28 */ u8 panMode;
                    /* 0x29 */ u8 panCurve;
                } SoundCommonInfo;

                typedef Util::DataRef<SoundCommonInfo> SoundCommonInfoRef;
                typedef Util::Table<SoundCommonInfoRef> SoundCommonTable;

                typedef struct BankInfo {
                    /* 0x00 */ u32 stringId;
                    /* 0x04 */ u32 fileId;
                } BankInfo;
                typedef Util::DataRef<BankInfo> BankInfoRef;
                typedef Util::Table<BankInfoRef> BankTable;

                typedef struct PlayerInfo {
                    /* 0x00 */ u32 stringId;
                    /* 0x04 */ u8 playableSoundCount;
                    /* 0x08 */ u32 heapSize;
                } PlayerInfo;
                typedef Util::DataRef<PlayerInfo> PlayerInfoRef;
                typedef Util::Table<PlayerInfoRef> PlayerTable;

                typedef Util::DataRef<SoundArchive::FilePos> SoundArchiveRef;
                typedef Util::Table<SoundArchiveRef> FilePosTable;

                typedef struct FileInfo {
                    /* 0x00 */ u32 fileSize;
                    /* 0x04 */ u32 waveDataSize;
                    /* 0x08 */ s32 entryNum;
                    /* 0x0C */ Util::DataRef<char> extFilePathRef;
                    /* 0x14 */ Util::DataRef<FilePosTable> filePosTableRef;
                } FileInfo;
                typedef Util::DataRef<FileInfo> FileInfoRef;
                typedef Util::Table<FileInfoRef> FileTable;

                typedef struct GroupItemInfo {
                    /* 0x0 */ u32 fileId;
                    /* 0x4 */ u32 offset;
                    /* 0x8 */ u32 size;
                    /* 0xC */ u32 waveDataOffset;
                    /* 0x10 */ u32 waveDataSize;
                } GroupItemInfo;
                typedef Util::DataRef<GroupItemInfo> GroupItemInfoRef;
                typedef Util::Table<GroupItemInfoRef> GroupItemTable;

                typedef struct GroupInfo {
                    /* 0x00 */ u32 stringId;
                    /* 0x04 */ s32 entryNum;
                    /* 0x08 */ Util::DataRef<char> extFilePathRef;
                    /* 0x10 */ u32 offset;
                    /* 0x14 */ u32 size;
                    /* 0x18 */ u32 waveDataOffset;
                    /* 0x1C */ u32 waveDataSize;
                    /* 0x20 */ Util::DataRef<GroupItemTable> itemTableRef;
                } GroupInfo;
                typedef Util::DataRef<GroupInfo> GroupInfoRef;
                typedef Util::Table<GroupInfoRef> GroupTable;

                typedef struct SoundArchivePlayerInfo {
                    /* 0x00 */ u16 seqSoundCount;
                    /* 0x02 */ u16 seqTrackCount;
                    /* 0x04 */ u16 strmSoundCount;
                    /* 0x06 */ u16 strmTrackCount;
                    /* 0x08 */ u16 strmChannelCount;
                    /* 0x0A */ u16 waveSoundCount;
                    /* 0x0C */ u16 waveTrackCount;
                } SoundArchivePlayerInfo;

                typedef struct Info {
                    /* 0x00 */ Util::DataRef<SoundCommonTable> soundTableRef;
                    /* 0x08 */ Util::DataRef<BankTable> bankTableRef;
                    /* 0x10 */ Util::DataRef<PlayerTable> playerTableRef;
                    /* 0x18 */ Util::DataRef<FileTable> fileTableRef;
                    /* 0x20 */ Util::DataRef<GroupTable> groupTableRef;
                    /* 0x28 */ Util::DataRef<SoundArchivePlayerInfo> soundArchivePlayerInfoRef;
                } Info;

                typedef struct InfoBlock {
                    /* 0x00 */ ut::BinaryBlockHeader blockHeader;
                    /* 0x08 */ Info info;
                } InfoBlock;

                typedef struct Header {
                    /* 0x00 */ ut::BinaryFileHeader fileHeader;
                    /* 0x10 */ u32 symbolDataOffset;
                    /* 0x14 */ u32 symbolDataSize;
                    /* 0x18 */ u32 infoOffset;
                    /* 0x1C */ u32 infoSize;
                    /* 0x20 */ u32 fileImageOffset;
                    /* 0x24 */ u32 fileImageSize;
                } Header;

                static const int HEADER_AREA_SIZE = OSRoundUp32B(sizeof(Header)) + 40;
            }  // namespace SoundArchiveFile

            class SoundArchiveFileReader {
            public:
                SoundArchiveFileReader();

                void Init(const void* soundArchiveData);
                bool IsValidFileHeader(const void* soundArchiveData);

                void SetStringChunk(const void* stringChunk, u32 size);
                void SetInfoChunk(const void* infoChunk, u32 size);

                SoundType GetSoundType(u32 id) const;

                bool ReadSoundInfo(u32 id, SoundArchive::SoundInfo* info) const;
                bool ReadSound3DParam(u32 id, SoundArchive::Sound3DParam* param) const;
                bool ReadSeqSoundInfo(u32 id, SoundArchive::SeqSoundInfo* info) const;
                bool ReadStrmSoundInfo(u32 id, SoundArchive::StrmSoundInfo* info) const;
                bool ReadWaveSoundInfo(u32 id, SoundArchive::WaveSoundInfo* info) const;
                bool ReadBankInfo(u32 id, SoundArchive::BankInfo* info) const;
                bool ReadPlayerInfo(u32 id, SoundArchive::PlayerInfo* info) const;
                bool ReadGroupInfo(u32 id, SoundArchive::GroupInfo* info) const;
                bool ReadGroupItemInfo(u32 groupId, u32 itemId,
                                       SoundArchive::GroupItemInfo* info) const;
                bool ReadSoundArchivePlayerInfo(SoundArchive::SoundArchivePlayerInfo* info) const;

                u32 GetSoundStringId(u32 id) const;
                u32 GetPlayerCount() const;
                u32 GetGroupCount() const;
                u32 GetFileCount() const;

                const char* GetSoundLabelString(u32 id) const;
                u32 GetSoundUserParam(u32 id) const;

                bool ReadFileInfo(u32 id, SoundArchive::FileInfo* info) const;
                bool ReadFilePos(u32 fileId, u32 id, SoundArchive::FilePos* filePos) const;

                const char* GetString(u32 id) const;

                u32 ConvertLabelStringToSoundId(const char* label) const {
                    return ConvertLabelStringToId(mStringTreeSound, label);
                }
                u32 ConvertLabelStringToPlayerId(const char* label) const {
                    return ConvertLabelStringToId(mStringTreePlayer, label);
                }
                u32 ConvertLabelStringToGroupId(const char* label) const {
                    return ConvertLabelStringToId(mStringTreeGroup, label);
                }

                u16 GetVersion() const { return mHeader.fileHeader.version; }
                u32 GetLabelStringChunkOffset() const { return mHeader.symbolDataOffset; }
                u32 GetLabelStringChunkSize() const { return mHeader.symbolDataSize; }
                u32 GetInfoChunkOffset() const { return mHeader.infoOffset; }

                u32 GetInfoChunkSize() const { return mHeader.infoSize; }

            private:
                u32 ConvertLabelStringToId(const SoundArchiveFile::StringTree* tree,
                                           const char* label) const;

                const SoundArchiveFile::SoundCommonInfo* impl_GetSoundInfo(u32 id) const;
                SoundArchiveFile::SoundInfoOffset impl_GetSoundInfoOffset(u32 id) const NO_INLINE;

                const SoundArchiveFile::SeqSoundInfo* impl_GetSeqSoundInfo(u32 id) const;
                const SoundArchiveFile::StrmSoundInfo* impl_GetStrmSoundInfo(u32 id) const;
                const SoundArchiveFile::WaveSoundInfo* impl_GetWaveSoundInfo(u32 id) const;

                const SoundArchiveFile::BankInfo* impl_GetBankInfo(u32 id) const;
                const SoundArchiveFile::PlayerInfo* impl_GetPlayerInfo(u32 id) const;
                const SoundArchiveFile::GroupInfo* impl_GetGroupInfo(u32 id) const;

                const void* GetPtrConst(const void* base, u32 offset) const {
                    if (offset == 0) {
                        return NULL;
                    }
                    return ut::AddOffsetToPtr(base, offset);
                }

            private:
                /* 0x00 */ SoundArchiveFile::Header mHeader;
                /* 0x28 */ const SoundArchiveFile::Info* mInfo;
                /* 0x2C */ const void* mStringBase;
                /* 0x30 */ const SoundArchiveFile::StringTable* mStringTable;
                /* 0x34 */ const SoundArchiveFile::StringTree* mStringTreeSound;
                /* 0x38 */ const SoundArchiveFile::StringTree* mStringTreePlayer;
                /* 0x3C */ const SoundArchiveFile::StringTree* mStringTreeGroup;
                /* 0x40 */ const SoundArchiveFile::StringTree* mStringTreeBank;
            };
        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
