#include "Font.h"

#include "../db/assert.h"
#include "CharStrmReader.h"

namespace nw4hbm {
    namespace ut {

        void Font::InitReaderFunc(FontEncoding encoding) {
            NW4HBM_ASSERT_VALID_PTR(43, this);

            switch (encoding) {
            case FONT_ENCODING_UTF8:
                mReaderFunc = &CharStrmReader::ReadNextCharUTF8;
                break;

            case FONT_ENCODING_UTF16:
                mReaderFunc = &CharStrmReader::ReadNextCharUTF16;
                break;

            case FONT_ENCODING_SJIS:
                mReaderFunc = &CharStrmReader::ReadNextCharSJIS;
                break;

            default:
            case FONT_ENCODING_CP1252:
                mReaderFunc = &CharStrmReader::ReadNextCharCP1252;
                break;
            }
        }

    }  // namespace ut
}  // namespace nw4hbm
