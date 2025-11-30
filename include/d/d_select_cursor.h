#ifndef D_D_SELECT_CURSOR_H
#define D_D_SELECT_CURSOR_H

#include "d/d_drawlist.h"
#include "d/d_select_icon.h"

class J2DAnmTransformKey;

class dSelect_cursorHIO_c {
public:
    dSelect_cursorHIO_c();

    virtual ~dSelect_cursorHIO_c() {}

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 field_0x8;
    /* 0x0C */ f32 mXAxisExpansion;
    /* 0x10 */ f32 mYAxisExpansion;
    /* 0x14 */ f32 mOscillation;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 mRatioX;
    /* 0x20 */ f32 mRatioY;
    /* 0x24 */ bool mDebugON;
};

class dSelect_cursor_c : public dDlst_base_c {
public:
    dSelect_cursor_c(u8, f32, JKRArchive*);
    void update();
    void setPos(f32, f32, J2DPane*, bool);
    void setParam(f32, f32, f32, f32, f32);
    void setScale(f32);
    void setAlphaRate(f32);
    int addAlpha();
    int decAlpha();
    void setBpkAnimation(J2DAnmColor*);
    void setBtk0Animation(J2DAnmTextureSRTKey*);
    void setCursorAnimation();
    void setBckAnimation(J2DAnmTransformKey*);
    void moveCenter(J2DPane*, f32, f32);

    virtual ~dSelect_cursor_c();
    virtual void draw();

    void setPos(f32 x, f32 y) {
        mPositionX = x;
        mPositionY = y;
    }

    void onUpdateFlag() { mUpdateFlag = true; }

    void resetUpdateFlag() { mUpdateFlag = false; }

    bool getUpdateFlag() { return mUpdateFlag; }

    void onPlayAnime(int i_flag) { field_0xb4 |= (1 << i_flag); }
    void offPlayAnime(int i_flag) { field_0xb4 &= ~(1 << i_flag); }
    bool chkPlayAnime(int i_flag) { return field_0xb4 & (1 << i_flag); }
    void onPlayAllAnime() { field_0xb4 = 0xff; }
    void offPlayAllAnime() { field_0xb4 = 0; }

private:
    /* 0x04 */ J2DScreen* mpScreen;
    /* 0x08 */ J2DPane* mpPane;
    /* 0x0C */ dSelect_icon_c* mpSelectIcon;
    /* 0x10 */ CPaneMgr* mpPaneMgr;
    /* 0x14 */ CPaneMgr* field_0x14;
    /* 0x18 */ CPaneMgr* field_0x18;
    /* 0x18 */ CPaneMgr* field_0x1C[4];
    /* 0x2C */ J2DAnmTransformKey* field_0x2C;
    /* 0x30 */ J2DAnmColor* field_0x30;
    /* 0x34 */ J2DAnmTextureSRTKey* field_0x34[2];
    /* 0x3C */ dSelect_cursorHIO_c* mpCursorHIO;
    /* 0x40 */ f32 field_0x40;
    /* 0x44 */ f32 field_0x44;
    /* 0x48 */ f32 field_0x48[2];
    /* 0x50 */ f32 field_0x50;
    /* 0x54 */ f32 field_0x54;
    /* 0x58 */ f32 mPositionX;
    /* 0x5C */ f32 mPositionY;
    /* 0x60 */ f32 mParam1;
    /* 0x64 */ f32 mParam2;
    /* 0x68 */ f32 mParam3;
    /* 0x6C */ f32 mParam4;
    /* 0x70 */ f32 mParam5;
    /* 0x74 */ f32 field_0x74[4];
    /* 0x84 */ f32 field_0x84[4];
    /* 0x94 */ f32 field_0x94[4];
    /* 0xA4 */ f32 field_0xa4[4];
    /* 0xB4 */ u8 field_0xb4;
    /* 0xB5 */ u8 mNameIdx;
    /* 0xB6 */ u8 field_0xb6;
    /* 0xB7 */ bool mUpdateFlag;
};

#endif /* D_D_SELECT_CURSOR_H */
