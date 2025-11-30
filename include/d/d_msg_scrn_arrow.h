#ifndef MSG_SCRN_D_MSG_SCRN_ARROW_H
#define MSG_SCRN_D_MSG_SCRN_ARROW_H

#include "dolphin/types.h"

class J2DScreen;
class J2DAnmTransform;
class J2DAnmColor;
class J2DScreen;
class CPaneMgr;

class dMsgScrnArrow_c {
public:
    dMsgScrnArrow_c();
    virtual ~dMsgScrnArrow_c();
    void draw();
    void setPos(f32, f32);
    void arwAnimeInit();
    void arwAnimeMove();
    void dotAnimeInit();
    void dotAnimeMove();

    /* 0x04 */ J2DScreen* mpScreen;
    /* 0x08 */ J2DAnmTransform* mpBck;
    /* 0x0C */ J2DAnmColor* mpBpk;
    /* 0x10 */ CPaneMgr* mpParent_c;
    /* 0x14 */ CPaneMgr* mpArw_c;
    /* 0x18 */ CPaneMgr* mpDot_c;
    /* 0x1C */ f32 mBckFrame;
    /* 0x20 */ f32 mBpkFrame;
};

#endif /* MSG_SCRN_D_MSG_SCRN_ARROW_H */
