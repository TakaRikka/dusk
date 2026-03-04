#ifndef NW4HBM_SND_SEQ_FILE_H
#define NW4HBM_SND_SEQ_FILE_H

#include <revolution/types.h>

#include "Util.h"

#include "../ut/binaryFileFormat.h"

#include <revolution/hbm.h>

namespace nw4hbm {
    namespace snd {
        namespace detail {
            namespace SeqFile {
                typedef struct Header {
                    /* 0x00 */ ut::BinaryFileHeader fileHeader;
                    /* 0x10 */ u32 dataBlockOffset;
                    /* 0x14 */ u32 dataBlockSize;
                    /* 0x18 */ u32 labelBlockOffset;
                    /* 0x1C */ u32 labelBlockSize;
                } Header;

                typedef struct DataBlock {
                    /* 0x00 */ ut::BinaryBlockHeader blockHeader;
                    /* 0x08 */ u32 baseOffset;
                } DataBlock;

                static const u32 SIGNATURE_DATA_BLOCK = 'DATA';
                static const u32 SIGNATURE_FILE = 'RSEQ';
                static const int FILE_VERSION = NW4HBM_VERSION(1, 0);
                static const int SUPPORTED_FILE_VERSION = NW4HBM_VERSION(1, 1);
            }  // namespace SeqFile

            class SeqFileReader {
            public:
                explicit SeqFileReader(const void* seqData);

                bool IsValidFileHeader(const void* seqData);

                const void* GetBaseAddress() const;

            private:
                /* 0x00 */ const SeqFile::Header* mHeader;
                /* 0x04 */ const SeqFile::DataBlock* mDataBlock;
            };

        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
