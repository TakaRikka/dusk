#ifndef NW4HBM_SND_WSD_FILE_H
#define NW4HBM_SND_WSD_FILE_H

#include <revolution/types.h>

#include "Util.h"
#include "WaveFile.h"
#include "snd_types.h"

#include "../ut/binaryFileFormat.h"

#include <revolution/hbm.h>

namespace nw4hbm {
    namespace snd {
        namespace detail {
            namespace WsdFile {

                static const u32 SIGNATURE_FILE = 'RWSD';
                static const u32 SIGNATURE_DATA_BLOCK = 'DATA';
                static const u32 SIGNATURE_WAVE_BLOCK = 'WAVE';
                static const int FILE_VERSION = NW4HBM_VERSION(1, 2);

                typedef struct WsdInfo {
                    /* 0x00 */ f32 pitch;
                    /* 0x04 */ u8 pan;
                    /* 0x05 */ u8 surroundPan;
                    /* 0x06 */ u8 fxSendA;
                    /* 0x07 */ u8 fxSendB;
                    /* 0x08 */ u8 fxSendC;
                    /* 0x09 */ u8 mainSend;
                    /* 0x0A */ u8 padding[2];
                    /* 0x0C */ Util::DataRef<void> graphEnvTablevRef;
                    /* 0x14 */ Util::DataRef<void> randomizerTableRef;
                    /* 0x1C */ u32 reserved;
                } WsdInfo;

                typedef struct TrackInfo {
                    // Nothing
                } TrackInfo;

                typedef struct NoteInfo {
                    /* 0x00 */ s32 waveIndex;
                    /* 0x04 */ u8 attack;
                    /* 0x05 */ u8 decay;
                    /* 0x06 */ u8 sustain;
                    /* 0x07 */ u8 release;
                    /* 0x08 */ u16 hold;
                    /* 0x0A */ u16 padding;
                    /* 0x0C */ u8 originalKey;
                    /* 0x0D */ u8 volume;
                    /* 0x0E */ u8 pan;
                    /* 0x0F */ u8 surroundPan;
                    /* 0x10 */ f32 pitch;
                    /* 0x14 */ Util::DataRef<void> lfoTableRef;
                    /* 0x1C */ Util::DataRef<void> graphEnvTablevRef;
                    /* 0x24 */ Util::DataRef<void> randomizerTableRef;
                    /* 0x2C */ u32 reserved;
                } NoteInfo;

                typedef Util::DataRef<TrackInfo> TrackInfoRef;
                typedef Util::Table<TrackInfoRef> TrackTable;

                typedef Util::DataRef<NoteInfo> NoteInfoRef;
                typedef Util::Table<NoteInfoRef> NoteTable;

                typedef struct Wsd {
                    /* 0x00 */ Util::DataRef<WsdInfo> refWsdInfo;
                    /* 0x08 */ Util::DataRef<TrackTable> refTrackTable;
                    /* 0x10 */ Util::DataRef<NoteTable> refNoteTable;
                } Wsd;

                typedef struct Header {
                    /* 0x00 */ ut::BinaryFileHeader fileHeader;
                    /* 0x10 */ u32 dataBlockOffset;
                    /* 0x14 */ u32 dataBlockSize;
                    /* 0x18 */ u32 waveBlockOffset;
                    /* 0x1C */ u32 waveBlockSize;
                } Header;

                typedef struct DataBlock {
                    /* 0x00 */ ut::BinaryBlockHeader blockHeader;
                    /* 0x08 */ u32 wsdCount;
                    /* 0x0C */ Util::DataRef<Wsd> refWsd[];
                } DataBlock;

                typedef struct WaveBlock {
                    /* 0x00 */ ut::BinaryBlockHeader blockHeader;
                    /* 0x08 */ u32 waveCount;
                    /* 0x0C */ u32 offsetTable[];
                } WaveBlock;

                // <= NW4HBM_VERSION(1, 0)
                typedef struct WaveBlockOld {
                    /* 0x00 */ ut::BinaryBlockHeader blockHeader;
                    /* 0x08 */ u32 offsetTable[];
                } WaveBlockOld;
            }  // namespace WsdFile

            typedef struct WaveSoundInfo {
                /* 0x00 */ f32 pitch;
                /* 0x04 */ u8 pan;
                /* 0x05 */ u8 surroundPan;
                /* 0x06 */ u8 fxSendA;
                /* 0x07 */ u8 fxSendB;
                /* 0x08 */ u8 fxSendC;
                /* 0x09 */ u8 mainSend;
            } WaveSoundInfo;

            typedef struct WaveSoundNoteInfo {
                /* 0x00 */ s32 waveIndex;
                /* 0x04 */ u8 attack;
                /* 0x06 */ u8 decay;
                /* 0x07 */ u8 sustain;
                /* 0x08 */ u8 release;
                /* 0x09 */ u8 originalKey;
                /* 0x0A */ u8 pan;
                /* 0x0B */ u8 surroundPan;
                /* 0x0C */ u8 volume;
                /* 0x10 */ f32 pitch;
            } WaveSoundNoteInfo;

            class WsdFileReader {
            public:
                explicit WsdFileReader(const void* waveData);

                bool IsValidFileHeader(const void* waveData);

                bool ReadWaveSoundInfo(WaveSoundInfo* soundInfo, int id) const;
                bool ReadWaveSoundNoteInfo(WaveSoundNoteInfo* soundNoteInfo, int id,
                                           int note) const;
                bool ReadWaveParam(int id, WaveData* waveData, const void* waveAddr) const;

            private:
                /* 0x00 */ const WsdFile::Header* mHeader;
                /* 0x04 */ const WsdFile::DataBlock* mDataBlock;
                /* 0x08 */ const WsdFile::WaveBlock* mWaveBlock;
            };
        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
