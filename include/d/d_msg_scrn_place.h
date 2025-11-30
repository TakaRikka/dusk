#ifndef MSG_SCRN_D_MSG_SCRN_PLACE_H
#define MSG_SCRN_D_MSG_SCRN_PLACE_H

#include "d/d_msg_scrn_base.h"

class dMsgScrnPlace_c : public dMsgScrnBase_c {
public:
    dMsgScrnPlace_c();

    virtual ~dMsgScrnPlace_c();
    virtual void exec();
    virtual void drawSelf();
    virtual void fukiAlpha(f32);
    virtual void fukiScale(f32);
    virtual void fukiTrans(f32, f32);
    virtual void fontAlpha(f32);

    /* 0xC4 */ CPaneMgr* mpBaseParent;
    /* 0xC8 */ CPaneMgr* mpFontParent;
    /* 0xCC */ f32 mScaleX;
    /* 0xD0 */ f32 mScaleY;
};

#endif /* MSG_SCRN_D_MSG_SCRN_PLACE_H */
