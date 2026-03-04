#ifndef NW4HBM_UT_FONT_H
#define NW4HBM_UT_FONT_H

#include <revolution/gx.h>
#include <revolution/types.h>

#include "CharStrmReader.h"
#include "fontResources.h"

#include "macros.h"
#include "global.h"

#define FONT_TYPE_NNGCTEXTURE 1
#define GLYPH_INDEX_NOT_FOUND 0xFFFF

namespace nw4hbm {
    namespace ut {

        typedef enum FontMapMethod {
            /* 0 */ FONT_MAPMETHOD_DIRECT = 0,
            /* 1 */ FONT_MAPMETHOD_TABLE,
            /* 2 */ FONT_MAPMETHOD_SCAN,
        } FontMapMethod;

        typedef enum FontEncoding {
            /* 0 */ FONT_ENCODING_UTF8 = 0,
            /* 1 */ FONT_ENCODING_UTF16,
            /* 2 */ FONT_ENCODING_SJIS,
            /* 3 */ FONT_ENCODING_CP1252,
            /* 4 */ NUM_OF_FONT_ENCODING
        } FontEncoding;

        class Font {
        public:
            typedef enum Type {
                /* 0xFFFF */ INVALID_CHARACTER_CODE = 0xFFFF,
                /* 0 */ TYPE_NULL = 0,
                /* 1 */ TYPE_ROM,
                /* 2 */ TYPE_RESOURCE,
            } Type;

        public:
            Font() : mReaderFunc(&CharStrmReader::ReadNextCharCP1252) {}

            /* 0x08 */ virtual ~Font() {}
            /* 0x0C */ virtual int GetWidth() const = 0;
            /* 0x10 */ virtual int GetHeight() const = 0;
            /* 0x14 */ virtual int GetAscent() const = 0;
            /* 0x18 */ virtual int GetDescent() const = 0;
            /* 0x1C */ virtual int GetBaselinePos() const = 0;
            /* 0x20 */ virtual int GetCellHeight() const = 0;
            /* 0x24 */ virtual int GetCellWidth() const = 0;
            /* 0x28 */ virtual int GetMaxCharWidth() const = 0;
            /* 0x2C */ virtual Type GetType() const = 0;
            /* 0x30 */ virtual GXTexFmt GetTextureFormat() const = 0;
            /* 0x34 */ virtual int GetLineFeed() const = 0;
            /* 0x38 */ virtual CharWidths GetDefaultCharWidths() const = 0;
            /* 0x3C */ virtual void SetDefaultCharWidths(const CharWidths& widths) = 0;
            /* 0x40 */ virtual bool SetAlternateChar(u16 c) = 0;
            /* 0x44 */ virtual void SetLineFeed(int linefeed) = 0;
            /* 0x48 */ virtual int GetCharWidth(u16 c) const = 0;
            /* 0x4C */ virtual CharWidths GetCharWidths(u16 c) const = 0;
            /* 0x50 */ virtual void GetGlyph(Glyph* glyph, u16 c) const = 0;
            /* 0x54 */ virtual FontEncoding GetEncoding() const = 0;

            void InitReaderFunc(FontEncoding encoding);

            CharStrmReader GetCharStrmReader() const NO_INLINE {
                //! TODO: required to make `ut_TextWriterBase.cpp` happy
                #ifndef NO_THIS_ASSERT
                NW4HBM_ASSERT_VALID_PTR(117, this);
                #endif
                CharStrmReader reader(mReaderFunc);
                return reader;
            }

        private:
            /* 0x00 (vtable) */
            /* 0x04 */ CharStrmReader::ReadFunc mReaderFunc;
        };

    }  // namespace ut
}  // namespace nw4hbm

#endif
