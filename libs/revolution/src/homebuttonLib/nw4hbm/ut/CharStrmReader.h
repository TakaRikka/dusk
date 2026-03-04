#ifndef NW4HBM_UT_CHAR_STREAM_READER_H
#define NW4HBM_UT_CHAR_STREAM_READER_H

// required to fix data order in `lyt_textBox.cpp`
#include "../lyt/textBox.h"
#include "../db/assert.h"

namespace nw4hbm {
    namespace ut {
        class CharStrmReader {
        public:
            typedef u16 (CharStrmReader::*ReadFunc)();

            CharStrmReader(ReadFunc func) : mCharStrm(NULL), mReadFunc(func) {}

            void Set(const char* stream) {
                NW4HBM_ASSERT_VALID_PTR(49, this);
                NW4HBM_ASSERT_VALID_PTR(50, stream);
                NW4HBM_ASSERT(53, mReadFunc == ReadNextCharUTF8 || mReadFunc == ReadNextCharCP1252 || mReadFunc == ReadNextCharSJIS);
                mCharStrm = stream;
            }

            void Set(const wchar_t* stream) {
                NW4HBM_ASSERT_VALID_PTR(59, this);
                NW4HBM_ASSERT_ALIGN2(60, stream);
                NW4HBM_ASSERT_VALID_PTR(61, stream);
                NW4HBM_ASSERT(62, mReadFunc == ReadNextCharUTF16);
                mCharStrm = stream;
            }

            const void* GetCurrentPos() const {
                NW4HBM_ASSERT_VALID_PTR(68, this);
                return mCharStrm;
            }

            u16 Next() {
                NW4HBM_ASSERT_VALID_PTR(74, this);
                return (this->*mReadFunc)();
            }

            u16 ReadNextCharUTF8();
            u16 ReadNextCharUTF16();
            u16 ReadNextCharCP1252();
            u16 ReadNextCharSJIS();

            template <typename T>
            T GetChar() const {
                const T* const charStrm = static_cast<const T* const>(mCharStrm);
                return charStrm[0];
            }

            template <typename T>
            T GetChar(int offset) const {
                const T* const charStrm = static_cast<const T* const>(mCharStrm);
                return charStrm[offset];
            }

            template <typename T>
            void StepStrm(int step) {
                const T*& charStrm = *reinterpret_cast<const T**>(&mCharStrm);
                charStrm += step;
            }

            /* 0x00 */ const void* mCharStrm;
            /* 0x04 */ const ReadFunc mReadFunc;
        };

    }  // namespace ut
}  // namespace nw4hbm

#endif
