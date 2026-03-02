#include "binaryFileFormat.h"

#include "../db/assert.h"

namespace nw4hbm {
    namespace ut {

        bool IsValidBinaryFile(const BinaryFileHeader* header, u32 signature, u16 version,
                               u16 minBlocks) {
            NW4HBM_ASSERT_VALID_PTR(48, header);

            if (header->signature != signature) {
                s8 signature1 = (header->signature >> 24) & 0xFF;
                s8 signature2 = (header->signature >> 16) & 0xFF;
                s8 signature3 = (header->signature >> 8) & 0xFF;
                s8 signature4 = (header->signature & 0xFF);

                s8 signature5 = (signature >> 24) & 0xFF;
                s8 signature6 = (signature >> 16) & 0xFF;
                s8 signature7 = (signature >> 8) & 0xFF;
                s8 signature8 = (signature & 0xFF);

                NW4R_DB_WARNING(60, false,
                                "Signature check failed ('%c%c%c%c' must be '%c%c%c%c').",
                                signature1, signature2, signature3, signature4, signature5,
                                signature6, signature7, signature8);
                return false;
            }

            // U+FEFF * BYTE ORDER MARK
            if (header->byteOrder != 0xFEFF) {
                NW4R_DB_WARNING(65, false, "Unsupported byte order.");
                return false;
            }

            if (header->version != version) {
                NW4R_DB_WARNING(75, false, "Version check faild ('%d.%d' must be '%d.%d').",
                                (header->version >> 8) & 0xFF, header->version & 0xFF,
                                (version >> 8) & 0xFF, version & 0xFF);
                return false;
            }

            if (header->fileSize < sizeof(*header) + sizeof(BinaryBlockHeader) * minBlocks) {
                NW4R_DB_WARNING(80, false, "Too small file size(=%d).", header->fileSize);
                return false;
            }

            if (header->dataBlocks < minBlocks) {
                NW4R_DB_WARNING(85, false, "Too small number of data blocks(=%d).",
                                header->dataBlocks);
                return false;
            }

            return true;
        }

        // required to match .data
        void dummy(const BinaryFileHeader* fileHeader) {
            NW4HBM_ASSERT_VALID_PTR(0, fileHeader);
        }

    }  // namespace ut
}  // namespace nw4hbm
