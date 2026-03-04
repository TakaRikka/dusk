#ifndef NW4HBM_UT_BINARY_FILE_FORMAT_H
#define NW4HBM_UT_BINARY_FILE_FORMAT_H

#include <revolution/types.h>

namespace nw4hbm {
    namespace ut {

        typedef struct BinaryFileHeader {
            /* 0x00 */ u32 signature;
            /* 0x04 */ u16 byteOrder;
            /* 0x06 */ u16 version;
            /* 0x08 */ u32 fileSize;
            /* 0x0C */ u16 headerSize;
            /* 0x0E */ u16 dataBlocks;
        } BinaryFileHeader;

        typedef struct BinaryBlockHeader {
            /* 0x00 */ u32 kind;
            /* 0x04 */ u32 size;
        } BinaryBlockHeader;

        bool IsValidBinaryFile(const BinaryFileHeader* header, u32 signature, u16 version,
                               u16 minBlocks);

    }  // namespace ut
}  // namespace nw4hbm

#endif
