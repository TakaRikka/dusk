#ifndef NW4HBM_UT_FONT_RESOURCES_H
#define NW4HBM_UT_FONT_RESOURCES_H

#include <revolution/types.h>

#include <revolution/gx.h>

namespace nw4hbm {
    namespace ut {

        typedef struct CharWidths {
            /* 0x00 */ s8 left;
            /* 0x01 */ u8 glyphWidth;
            /* 0x02 */ s8 charWidth;
        } CharWidths;  // size = 0x04

        typedef struct FontWidth {
            /* 0x00 */ u16 indexBegin;
            /* 0x02 */ u16 indexEnd;
            /* 0x04 */ FontWidth* pNext;
            /* 0x08 */ CharWidths widthTable[];
        } FontWidth;

        typedef struct Glyph {
            /* 0x00 */ void* pTexture;
            /* 0x04 */ CharWidths widths;
            /* 0x07 */ u8 height;
            /* 0x08 */ GXTexFmt texFormat;
            /* 0x0C */ u16 texWidth;
            /* 0x0E */ u16 texHeight;
            /* 0x10 */ u16 cellX;
            /* 0x12 */ u16 cellY;
        } Glyph;  // size = 0x14

        typedef struct FontTextureGlyph {
            /* 0x00 */ u8 cellWidth;
            /* 0x01 */ u8 cellHeight;
            /* 0x02 */ s8 baselinePos;
            /* 0x03 */ u8 maxCharWidth;
            /* 0x04 */ u32 sheetSize;
            /* 0x08 */ u16 sheetNum;
            /* 0x0A */ u16 sheetFormat;
            /* 0x0C */ u16 sheetRow;
            /* 0x0E */ u16 sheetLine;
            /* 0x10 */ u16 sheetWidth;
            /* 0x12 */ u16 sheetHeight;
            /* 0x14 */ u8* sheetImage;
        } FontTextureGlyph;  // size = 0x18

        typedef struct FontCodeMap {
            /* 0x00 */ u16 ccodeBegin;
            /* 0x02 */ u16 ccodeEnd;
            /* 0x04 */ u16 mappingMethod;
            /* 0x06 */ u16 reserved;
            /* 0x08 */ FontCodeMap* pNext;
            /* 0x0C */ u16 mapInfo[];
        } FontCodeMap;

        typedef struct FontInformation {
            /* 0x00 */ u8 fontType;
            /* 0x01 */ s8 linefeed;
            /* 0x02 */ u16 alterCharIndex;
            /* 0x04 */ CharWidths defaultWidth;
            /* 0x07 */ u8 encoding;
            /* 0x08 */ FontTextureGlyph* pGlyph;
            /* 0x0C */ FontWidth* pWidth;
            /* 0x10 */ FontCodeMap* pMap;
            /* 0x14 */ u8 height;
            /* 0x15 */ u8 width;
            /* 0x16 */ u8 ascent;
            /* 0x17 */ u8 padding_[1];
        } FontInformation;  // size = 0x18

    }  // namespace ut
}  // namespace nw4hbm

#endif
