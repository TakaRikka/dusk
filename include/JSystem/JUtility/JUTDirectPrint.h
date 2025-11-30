#ifndef JUTDIRECTPRINT_H
#define JUTDIRECTPRINT_H

#include "JSystem/JUtility/TColor.h"
#include "__va_arg.h"

/**
* @ingroup jsystem-jutility
* 
*/
class JUTDirectPrint {
private:
    JUTDirectPrint();

public:
    void erase(int, int, int, int);
    void drawChar(int, int, int);
    void changeFrameBuffer(void*, u16, u16);
    /* -------- */ void print(u16, u16, char const*, ...);
    void printSub(u16, u16, char const*, va_list, bool);
    void drawString(u16, u16, char*);
    void drawString_f(u16, u16, char const*, ...);
    void setCharColor(u8, u8, u8);
    void setCharColor(JUtility::TColor);

    static JUTDirectPrint* start();

    bool isActive() const { return field_0x00 != 0; }
    JUtility::TColor getCharColor() const { return mCharColor; }
    void changeFrameBuffer(void *param_0) { changeFrameBuffer(param_0, mFrameBufferWidth, mFrameBufferHeight); }
    void* getFrameBuffer() { return field_0x00; }

    static JUTDirectPrint* getManager() { return sDirectPrint; }

private:
    static u8 sAsciiTable[128];
    static u32 sFontData[64];
    static u32 sFontData2[77];
    static JUTDirectPrint* sDirectPrint;
    static u8 sDirectPrint_padding[4 /* padding */];

private:
    /* 0x00 */ void* field_0x00;
    /* 0x04 */ u16 mFrameBufferWidth;
    /* 0x06 */ u16 mFrameBufferHeight;
    /* 0x08 */ u16 mStride;
    /* 0x0A */ u16 field_0x0A;
    /* 0x0C */ size_t mFrameBufferSize;
    /* 0x10 */ u8 field_0x10[4];
    /* 0x14 */ u16* mFrameBuffer;
    /* 0x18 */ JUtility::TColor mCharColor;
    /* 0x1C */ u16 mCharColor_Y;
    /* 0x1E */ u16 mCharColor_Cb;
    /* 0x20 */ u16 mCharColor_Cb2;
    /* 0x22 */ u16 mCharColor_Cb4;
    /* 0x24 */ u16 mCharColor_Cr;
    /* 0x26 */ u16 mCharColor_Cr2;
    /* 0x28 */ u16 mCharColor_Cr4;
    /* 0x2A */ u16 field_0x2A;
    /* 0x2C */
};

#endif /* JUTDIRECTPRINT_H */
