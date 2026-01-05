#ifndef JUTFONT_H
#define JUTFONT_H

#include "JSystem/JUtility/TColor.h"
#include <string>

/**
* @ingroup jsystem-jutility
* 
*/
struct ResFONT {
    struct INF1 {
        /* 0x00 */ u32 magic;
        /* 0x04 */ u32 size;
        /* 0x08 */ u16 fontType;
        /* 0x0A */ u16 ascent;
        /* 0x0C */ u16 descent;
        /* 0x0E */ u16 width;
        /* 0x10 */ u16 leading;
        /* 0x12 */ u16 defaultCode;
    };

    struct WID1 {
        /* 0x00 */ u32 magic;
        /* 0x04 */ u32 size;
        /* 0x08 */ u16 startCode;
        /* 0x0A */ u16 endCode;
        /* 0x0C */ u8 mChunkNum[4];
    };

    struct MAP1 {
        /* 0x00 */ u32 magic;
        /* 0x04 */ u32 size;
        /* 0x08 */ u16 mappingMethod;
        /* 0x0A */ u16 startCode;
        /* 0x0C */ u16 endCode;
        /* 0x0E */ u16 numEntries;
        /* 0x10 */ u16 mLeading;
    };

    struct GLY1 {
        /* 0x00 */ u32 magic;
        /* 0x04 */ u32 size;
        /* 0x08 */ u16 startCode;
        /* 0x0A */ u16 endCode;
        /* 0x0C */ u16 cellWidth;
        /* 0x0E */ u16 cellHeight;
        /* 0x10 */ u32 textureSize;
        /* 0x14 */ u16 textureFormat;
        /* 0x16 */ u16 numRows;
        /* 0x18 */ u16 numColumns;
        /* 0x1A */ u16 textureWidth;
        /* 0x1C */ u16 textureHeight;
        /* 0x1E */ u16 padding;
        /* 0x20 */ u8 data[];
    };

    /* 0x00 */ u64 magic;
    /* 0x08 */ u32 filesize;
    /* 0x0C */ u32 numBlocks;
    /* 0x10 */ u8 padding[0x10];
    /* 0x20 */ u8 data[];
};

/**
* @ingroup jsystem-jutility
* 
*/
class JUTFont {
public:
    JUTFont();
    virtual ~JUTFont() {}

    struct TWidth {
        u8 field_0x0;
        u8 field_0x1;
    };

    /* 0x0C */ virtual void setGX() = 0;
    /* 0x10 */ virtual void setGX(JUtility::TColor col1, JUtility::TColor col2) { setGX(); }
    /* 0x14 */ virtual f32 drawChar_scale(f32 a1, f32 a2, f32 a3, f32 a4, int a5, bool a6) = 0;
    /* 0x18 */ virtual int getLeading() const = 0;
    /* 0x1C */ virtual s32 getAscent() const = 0;
    /* 0x20 */ virtual s32 getDescent() const = 0;
    /* 0x24 */ virtual s32 getHeight() const = 0;
    /* 0x28 */ virtual s32 getWidth() const = 0;
    /* 0x2C */ virtual void getWidthEntry(int i_no, TWidth* width) const = 0;
    /* 0x30 */ virtual s32 getCellWidth() const { return getWidth(); }
    /* 0x34 */ virtual s32 getCellHeight() const { return getHeight(); }
    /* 0x38 */ virtual int getFontType() const = 0;
    /* 0x3C */ virtual ResFONT* getResFont() const = 0;
    /* 0x40 */ virtual bool isLeadByte(int a1) const = 0;

    static bool isLeadByte_1Byte(int b) {
        return false;
    }
    static bool isLeadByte_2Byte(int b) {
        return true;
    }
    static bool isLeadByte_ShiftJIS(int b) {
        return (b >= 0x81 && b <= 0x9f) || (b >= 0xe0 && b <= 0xfc);
    }
    static bool isLeadByte_EUC(int b) {
        return (b >= 0xA1 && b <= 0xFE) || b == 0x8E;
    }

    void initialize_state();
    void setCharColor(JUtility::TColor col1);
    void setGradColor(JUtility::TColor col1, JUtility::TColor col2);
    f32 drawString_size_scale(f32 posX, f32 posY, f32 width, f32 height, const char* str, u32 usz,
                              bool visible);

    void drawString(int posX, int posY, const char* str, bool visible) {
        drawString_size(posX, posY, str, strlen(str), visible);
    }

    s32 drawString_size(int posX, int posY, const char* str, u32 len, bool visible) {
        return drawString_size_scale(posX, posY, getWidth(), getHeight(), str, len, visible);
    }

    void drawString_scale(f32 posX, f32 posY, f32 width, f32 height, const char* str,
                          bool visible) {
        drawString_size_scale(posX, posY, width, height, str, strlen(str), visible);
    }

    int getWidth(int i_no) const {
        TWidth width;
        getWidthEntry(i_no, &width);
        return width.field_0x1;
    }

    bool isValid() const { return mValid; }
    bool isFixed() const { return mFixed; }
    int getFixedWidth() const { return mFixedWidth; }
    void setFixedWidth(bool fixed, int width) {
        mFixed = fixed;
        mFixedWidth = width;
    }
    int getOffset(int i_no) const { 
        JUTFont::TWidth width;
        getWidthEntry(i_no, &width);
        return width.field_0x0;
    }

    /* 0x04 */ bool mValid;
    /* 0x05 */ bool mFixed;
    /* 0x08 */ int mFixedWidth;
    /* 0x0C */ JUtility::TColor mColor1;
    /* 0x10 */ JUtility::TColor mColor2;
    /* 0x14 */ JUtility::TColor mColor3;
    /* 0x18 */ JUtility::TColor mColor4;
};

#endif /* JUTFONT_H */
