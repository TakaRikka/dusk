#ifndef D_MSG_D_MSG_OUT_FONT_H
#define D_MSG_D_MSG_OUT_FONT_H

#include "dolphin/types.h"

class J2DPicture;
class J2DTextBox;

class COutFontSet_c {
public:
    COutFontSet_c();
    void initialize();
    void drawFont(J2DTextBox*, u8, f32, f32, f32, f32, u32, u8);

    virtual ~COutFontSet_c();

    u8 getType() { return mType; }
    u32 getColor() { return mColor; }
    f32 getPosX() { return mPosX; }
    f32 getPosY() { return mPosY; }
    f32 getSizeX() { return mSizeX; }
    f32 getSizeY() { return mSizeY; }
    u8 getAlpha() { return mAlpha; }
    J2DTextBox* getTextBoxPtr() { return mpTextBoxPtr; }
    void setTextBoxPtr(J2DTextBox* p_textBox) { mpTextBoxPtr = p_textBox; }
    void resetType() { mType = 0x47; }

private:
    /* 0x04 */ J2DTextBox* mpTextBoxPtr;
    /* 0x08 */ f32 mPosX;
    /* 0x0C */ f32 mPosY;
    /* 0x10 */ f32 mSizeX;
    /* 0x14 */ f32 mSizeY;
    /* 0x18 */ u32 mColor;
    /* 0x1C */ u8 mAlpha;
    /* 0x1D */ u8 mType;
};

class COutFont_c {
public:
    COutFont_c(u8);
    void initialize();
    void setBlendAnime(J2DPicture*, s16);
    const char* getBtiName(int);

    virtual ~COutFont_c();
    virtual void setPane(J2DPicture*);
    virtual void createPane();
    virtual void draw(J2DTextBox*, f32, f32, f32);
    virtual void reset(J2DTextBox*);
    virtual void drawFont(J2DTextBox*, u8, f32, f32, f32, f32, u32, u8);
    virtual void setAlphaRatio(f32);

    void setRupeeColor(u8 color) { mRupeeColor = color; }

private:
    /* 0x004 */ COutFontSet_c* mpOfs[35];
    /* 0x090 */ J2DPicture* mpPane[70];
    /* 0x1A8 */ f32 mAlphaRatio;
    /* 0x1AC */ f32 field_0x1ac;
    /* 0x1B0 */ f32 field_0x1b0;
    /* 0x1B4 */ s16 field_0x1b4[70];
    /* 0x240 */ bool field_0x240;
    /* 0x241 */ u8 mRupeeColor;
    /* 0x242 */ u8 field_0x242;
};

#endif /* D_MSG_D_MSG_OUT_FONT_H */
