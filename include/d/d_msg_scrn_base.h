#ifndef MSG_SCRN_D_MSG_SCRN_BASE_H
#define MSG_SCRN_D_MSG_SCRN_BASE_H

#include "JSystem/J2DGraph/J2DTextBox.h"
#include "d/d_drawlist.h"

class COutFont_c;
class CPaneMgr;
struct CharInfo_c;

class dMsgScrnBase_c : public dDlst_base_c {
public:
    dMsgScrnBase_c();
    void init();
    void drawOutFont(f32, f32, f32);
    void setString(char*, char*);
    void setRubyString(char*);
    bool isTalkNow();

    virtual void draw();
    virtual ~dMsgScrnBase_c();
    virtual void exec() {}
    virtual void multiDraw();
    virtual void drawSelf();
    virtual void setSelectString(char*, char*, char*) {}
    virtual void setSelectRubyString(char*, char*, char*) {}
    virtual void arwAnimeInit() {}
    virtual void arwAnimeMove() {}
    virtual void dotAnimeInit() {}
    virtual void dotAnimeMove() {}
    virtual bool isSelect() { return true; }
    virtual void selectAnimeInit(u8, u8, f32, u8) {}
    virtual bool selectAnimeMove(u8, u8, bool) { return true; }
    virtual bool selectAnimeEnd() { return true; }
    virtual void fukiScale(f32);
    virtual void fukiTrans(f32, f32);
    virtual void fukiAlpha(f32);
    virtual void fontAlpha(f32);
    virtual void fukiPosCalc(u8) {}

    f32 getSelTextBoxPosX(int idx) { return mSelTextBoxPosX[idx]; }
    f32 getSelTextBoxPosY(int idx) { return mSelTextBoxPosY[idx]; }
    f32 getTextBoxPosX() { return mTextBoxPosX; }
    f32 getTextBoxPosY() { return mTextBoxPosY; }
    f32 getTextBoxScaleX() { return mTextBoxScaleX; }
    f32 getTextBoxScaleY() { return mTextBoxScaleY; }
    f32 getTextBoxPosOffsetY() { return mTextBoxPosOffsetY; }
    f32 getFontSizeX() { return mFontSize.mSizeX; }
    f32 getFontSizeY() { return mFontSize.mSizeY; }
    f32 getRubySize() { return mRubySize; }
    f32 getTBoxWidth() { return mTBoxWidth; }
    f32 getTBoxHeight() { return mTBoxHeight; }
    f32 getLineSpace() { return mLineSpace; }
    f32 getCharSpace() { return mCharSpace; }
    f32 getRubyCharSpace() { return mRubyCharSpace; }
    f32 getSelFontSize() { return mSelFontSize; }
    f32 getSelRubySize() { return mSelRubySize; }
    f32 getSelTBoxWidth() { return mSelTBoxWidth; }
    f32 getSelCharSpace() { return mSelCharSpace; }
    f32 getSelRubyCharSpace() { return mSelRubyCharSpace; }
    CharInfo_c* getCharInfoPtr() { return mCharInfoPtr; }
    void setCharAlphaRate(f32 rate) { mCharAlphaRate = rate; }
    void setTextBoxPosOffsetY(f32 offset) { mTextBoxPosOffsetY = offset; }
    void setMsgID(u16 msgID) { mMsgID = msgID; }
    void setOutFont(COutFont_c* font) { mpOutFont = font; }

    /* 0x04 */ J2DScreen* mpScreen;
    /* 0x08 */ CPaneMgr* mpPmP_c;
    /* 0x0C */ CPaneMgr* mpTm_c[7];
    /* 0x28 */ CPaneMgr* mpTmr_c[7];
    /* 0x44 */ COutFont_c* mpOutFont;
    /* 0x48 */ dDlst_base_c* field_0x48;
    /* 0x4C */ u32 field_0x4c;
    /* 0x50 */ CharInfo_c* mCharInfoPtr;
    /* 0x54 */ JUTFont* field_0x54;
    /* 0x58 */ J2DTextBox::TFontSize mFontSize;
    /* 0x60 */ f32 mRubySize;
    /* 0x64 */ f32 mTextBoxPosX;
    /* 0x68 */ f32 mTextBoxPosY;
    /* 0x6C */ f32 mTextBoxScaleX;
    /* 0x70 */ f32 mTextBoxScaleY;
    /* 0x74 */ f32 mTBoxWidth;
    /* 0x78 */ f32 mTBoxHeight;
    /* 0x7C */ f32 mLineSpace;
    /* 0x80 */ f32 mCharSpace;
    /* 0x84 */ f32 mRubyCharSpace;
    /* 0x88 */ f32 mSelFontSize;
    /* 0x8C */ f32 mSelRubySize;
    /* 0x90 */ f32 mSelTextBoxPosX[3];
    /* 0x9C */ f32 mSelTextBoxPosY[3];
    /* 0xA8 */ f32 mSelTBoxWidth;
    /* 0xAC */ f32 mSelCharSpace;
    /* 0xB0 */ f32 mSelRubyCharSpace;
    /* 0xB4 */ f32 mTextBoxPosOffsetY;
    /* 0xB8 */ f32 field_0xb8;
    /* 0xBC */ f32 mCharAlphaRate;
    /* 0xC0 */ u16 mMsgID;
};

#endif /* MSG_SCRN_D_MSG_SCRN_BASE_H */
