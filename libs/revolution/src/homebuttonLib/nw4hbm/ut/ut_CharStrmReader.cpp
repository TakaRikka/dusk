#include "CharStrmReader.h"

#include "../db/assert.h"

namespace nw4hbm {
    namespace ut {

        inline bool IsSJISLeadByte(u8 c) {
            return (0x81 <= c && c < 0xA0) || 0xE0 <= c;
        }

        u16 CharStrmReader::ReadNextCharUTF8() {
            NW4HBM_ASSERT_VALID_PTR(76, this);
            NW4HBM_ASSERT_VALID_PTR(77, mCharStrm);
            NW4HBM_ASSERT(79, (GetChar<u8>() & 0xC0) != 0x80);
            u16 code;

            if ((GetChar<u8>(0) & 0x80) == 0x00) {
                // 1-byte UTF-8 sequence
                code = GetChar<u8>(0);
                StepStrm<u8>(1);
            } else if ((GetChar<u8>(0) & 0xE0) == 0xC0) {
                // 2-byte UTF-8 sequence
                code = (GetChar<u8>(0) & 0x1f) << 6 | (GetChar<u8>(1) & 0x3f);
                StepStrm<u8>(2);
            } else {
                // 3-byte UTF-8 sequence

                NW4HBM_ASSERT(100, (GetChar<u8>() & 0xF0) == 0xE0);
                /* technical ERRATUM: the mask of GetChar<u8>(0) should be 0x0f */
                code = (GetChar<u8>(0) & 0x1f) << 12 | (GetChar<u8>(1) & 0x3f) << 6 |
                       (GetChar<u8>(2) & 0x3f);
                StepStrm<u8>(3);
            }

            /* NOTE: 4-byte to 7-byte UTF-8 sequences usually encode code points outside
             * of the BMP; I think sticking to the BMP is fine here.
             */

            return code;
        }

        u16 CharStrmReader::ReadNextCharUTF16() {
            NW4HBM_ASSERT_VALID_PTR(129, this);
            NW4HBM_ASSERT_VALID_PTR(130, mCharStrm);
            NW4HBM_ASSERT_ALIGN2(131, mCharStrm);
            u16 code = GetChar<u16>(0);
            StepStrm<u16>(1);

            return code;
        }

        u16 CharStrmReader::ReadNextCharCP1252() {
            NW4HBM_ASSERT_VALID_PTR(155, this);
            NW4HBM_ASSERT_VALID_PTR(156, mCharStrm);
            u16 code = GetChar<u8>(0);
            StepStrm<u8>(1);

            return code;
        }

        u16 CharStrmReader::ReadNextCharSJIS() {
            NW4HBM_ASSERT_VALID_PTR(180, this);
            NW4HBM_ASSERT_VALID_PTR(181, mCharStrm);
            u16 code;

            if (IsSJISLeadByte(GetChar<u8>(0))) {
                code = GetChar<u8>(0) << 8 | GetChar<u8>(1);
                StepStrm<u8>(2);
            } else {
                code = GetChar<u8>(0);
                StepStrm<u8>(1);
            }

            return code;
        }

    }  // namespace ut
}  // namespace nw4hbm
