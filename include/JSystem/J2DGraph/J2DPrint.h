#ifndef J2DPRINT_H
#define J2DPRINT_H

#include "JSystem/J2DGraph/J2DTextBox.h"
#include "__va_arg.h"

class JUTFont;
class J2DPrint;

extern "C" f32 J2DPrint_print_alpha_va(J2DPrint*, u8, const char*, va_list);

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DPrint {
public:
    struct TSize {
        /* 0x0 */ f32 field_0x0;
        /* 0x4 */ f32 field_0x4;
    };

    J2DPrint(JUTFont*, JUtility::TColor, JUtility::TColor);

    void initiate();

    J2DPrint(JUTFont*, f32, f32, JUtility::TColor, JUtility::TColor, JUtility::TColor, JUtility::TColor);
    static u8* setBuffer(size_t);
    void setFontSize();
    void locate(f32, f32);
    f32 print(f32, f32, u8, char const*, ...);
    void printReturn(char const*, f32, f32, J2DTextBoxHBinding, J2DTextBoxVBinding,
                                    f32, f32, u8);
    f32 parse(u8 const*, int, int, u16*, J2DPrint::TSize&, u8, bool);
    void doCtrlCode(int);
    u16 doEscapeCode(u8 const**, u8);
    void initchar();
    s32 getNumberS32(u8 const**, s32, s32, int);
    f32 getNumberF32(u8 const**, f32, f32, int);

    virtual ~J2DPrint();

    f32 print_va(u8 alpha, const char* fmt, va_list args) {
        return J2DPrint_print_alpha_va(this, alpha, fmt, args);
    }

    void setFontSize(f32 sizeX, f32 sizeY) {
        f32 x;
        if (sizeX > 0.0f) {
            x = sizeX;
        } else {
            x = 0.0f;
        }
        mFontSizeX = x;

        f32 y;
        if (sizeY > 0.0f) {
            y = sizeY;
        } else {
            y = 0.0f;
        }
        mFontSizeY = y;
    }

    static char* mStrBuff;
    static size_t mStrBuffSize;

private:
    void private_initiate(JUTFont*, f32, f32, JUtility::TColor, JUtility::TColor,
                                         JUtility::TColor, JUtility::TColor, bool);

    /* 0x04 */ JUTFont* mFont;
    /* 0x08 */ JUtility::TColor field_0x8;
    /* 0x0C */ JUtility::TColor field_0xc;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 mScaleX;
    /* 0x1C */ f32 mScaleY;
    /* 0x20 */ s16 field_0x20;
    /* 0x22 */ bool field_0x22;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 mCursorH;
    /* 0x30 */ f32 mCursorV;
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ JUtility::TColor mBlackColor;
    /* 0x3C */ JUtility::TColor mWhiteColor;
    /* 0x40 */ JUtility::TColor mCharColor;
    /* 0x44 */ JUtility::TColor mGradColor;
    /* 0x48 */ f32 mCharSpacing;
    /* 0x4C */ f32 mLineSpacing;
    /* 0x50 */ f32 mFontSizeX;
    /* 0x54 */ f32 mFontSizeY;
    /* 0x58 */ s16 field_0x58;
    /* 0x5A */ bool field_0x5a;
};  // Size: 0x5C

#endif /* J2DPRINT_H */
