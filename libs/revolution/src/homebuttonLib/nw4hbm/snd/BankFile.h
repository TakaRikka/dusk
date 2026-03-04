#ifndef NW4HBM_SND_BANK_FILE_H
#define NW4HBM_SND_BANK_FILE_H

#include <revolution/types.h>

#include "Util.h"
#include "WaveFile.h"

#include "../ut/binaryFileFormat.h"

#include <revolution/hbm.h>

namespace nw4hbm {
    namespace snd {
        namespace detail {

            inline u8 ReadByte(const void* address) {
                return *static_cast<const u8*>(address);
            }

            namespace BankFile {

                static const u32 SIGNATURE_FILE = 'RBNK';
                static const u32 SIGNATURE_DATA_BLOCK = 'DATA';
                static const u32 SIGNATURE_WAVE_BLOCK = 'WAVE';

                typedef struct InstParam {
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
                    /* 0x0F */ u8 padding2;
                    /* 0x10 */ f32 tune;
                    /* 0x14 */ Util::DataRef<void> lfoTableRef;
                    /* 0x1C */ Util::DataRef<void> graphEnvTablevRef;
                    /* 0x24 */ Util::DataRef<void> randomizerTableRef;
                    /* 0x2C */ u32 reserved;
                } InstParam;

                typedef struct RangeTable {
                    /* 0x00 */ u8 tableSize;
                    /* 0x01 */ u8 key[];
                } RangeTable;

                typedef struct IndexTable IndexTable;

                typedef Util::DataRef<void, InstParam, RangeTable, IndexTable> DataRegion;
                typedef Util::DataRef<WaveFile::WaveInfo> WaveRegion;

                struct IndexTable {
                    /* 0x00 */ u8 min;
                    /* 0x01 */ u8 max;
                    /* 0x02 */ u16 reserved;
                    /* 0x04 */ DataRegion ref[];
                };

                typedef struct Header {
                    /* 0x00 */ ut::BinaryFileHeader fileHeader;
                    /* 0x10 */ u32 dataBlockOffset;
                    /* 0x14 */ u32 dataBlockSize;
                    /* 0x18 */ u32 waveBlockOffset;
                    /* 0x1C */ u32 waveBlockSize;
                } Header;

                typedef struct DataBlock {
                    /* 0x00 */ ut::BinaryBlockHeader blockHeader;
                    /* 0x08 */ Util::Table<DataRegion> instTable;
                } DataBlock;

                typedef struct WaveBlock {
                    /* 0x00 */ ut::BinaryBlockHeader blockHeader;
                    /* 0x08 */ Util::Table<WaveRegion> waveInfoTable;
                } WaveBlock;

            }  // namespace BankFile

            typedef struct InstInfo {
                /* 0x00 */ s32 waveIndex;
                /* 0x04 */ u8 attack;
                /* 0x05 */ u8 decay;
                /* 0x06 */ u8 sustain;
                /* 0x07 */ u8 release;
                /* 0x08 */ u8 originalKey;
                /* 0x09 */ u8 pan;
                /* 0x0A */ u8 volume;
                /* 0x0C */ f32 tune;
            } InstInfo;

            class BankFileReader {
            public:
                static const int FILE_VERSION = NW4HBM_VERSION(1, 1);

            public:
                explicit BankFileReader(const void* bankData);

                bool IsValidFileHeader(const void* bankData);

                bool ReadInstInfo(InstInfo* instInfo, int prgNo, int key, int velocity) const;
                bool ReadWaveParam(WaveData* waveParam, int waveIndex,
                                   const void* waveDataAddress) const;

            private:
                const BankFile::DataRegion* GetReferenceToSubRegion(const BankFile::DataRegion* ref,
                                                                    int splitKey) const;

            private:
                /* 0x00 */ const BankFile::Header* mHeader;
                /* 0x04 */ const BankFile::DataBlock* mDataBlock;
                /* 0x08 */ const BankFile::WaveBlock* mWaveBlock;
            };

        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
