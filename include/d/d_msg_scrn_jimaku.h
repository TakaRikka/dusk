#ifndef MSG_SCRN_D_MSG_SCRN_JIMAKU_H
#define MSG_SCRN_D_MSG_SCRN_JIMAKU_H

#include "d/d_msg_scrn_base.h"
#include "JSystem/JKernel/JKRExpHeap.h"

class dMsgScrnLight_c;

class dMsgScrnJimaku_c : public dMsgScrnBase_c {
public:
    dMsgScrnJimaku_c(u8, JKRExpHeap*);
    virtual ~dMsgScrnJimaku_c();
    virtual void exec();
    virtual void drawSelf();
    virtual void fukiScale(f32);
    virtual void fukiAlpha(f32);
    virtual void fukiTrans(f32, f32);
    virtual void fontAlpha(f32);

    /* 0xC4 */ dMsgScrnLight_c* mpLight_c;
    /* 0xC8 */ JKRExpHeap* heap;
    /* 0xCC */ f32 field_0xcc;
};

#endif /* MSG_SCRN_D_MSG_SCRN_JIMAKU_H */
