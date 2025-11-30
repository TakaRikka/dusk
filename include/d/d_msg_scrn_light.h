#ifndef MSG_SCRN_D_MSG_SCRN_LIGHT_H
#define MSG_SCRN_D_MSG_SCRN_LIGHT_H

#include "JSystem/JUtility/TColor.h"

class J2DScreen;
class J2DAnmTransform;
class J2DAnmColorKey;
class CPaneMgr;

class dMsgScrnLight_c {
public:
    dMsgScrnLight_c(u8, u8);
    void draw(f32*, f32, f32, f32, f32, f32, u8);
    void draw(f32*, f32, f32, f32, f32, f32, f32, JUtility::TColor,
                             JUtility::TColor);
    void drawCommon(f32, f32, f32, f32, f32);

    virtual ~dMsgScrnLight_c();

    /* 0x04 */ J2DScreen* mpScreen;
    /* 0x08 */ J2DAnmTransform* mpBck;
    /* 0x0C */ J2DAnmColorKey* mpBpk;
    /* 0x10 */ CPaneMgr* mpParent_c;
    /* 0x14 */ f32 mBckFrame;
    /* 0x18 */ f32 mBpkFrame;
    /* 0x1C */ f32 mAlpha;
    /* 0x20 */ u8 mColorType;
    /* 0x21 */ u8 field_0x21;
    /* 0x22 */ bool mPlayAnim;
};

#endif /* MSG_SCRN_D_MSG_SCRN_LIGHT_H */
