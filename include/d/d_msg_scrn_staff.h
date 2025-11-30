#ifndef MSG_SCRN_D_MSG_SCRN_STAFF_H
#define MSG_SCRN_D_MSG_SCRN_STAFF_H

#include "d/d_msg_scrn_base.h"

class dMsgScrnStaff_c : public dMsgScrnBase_c {
public:
    dMsgScrnStaff_c(u8);

    virtual ~dMsgScrnStaff_c();
    virtual void exec();
    virtual void drawSelf();
    virtual void fukiAlpha(f32);
    virtual void fukiScale(f32);
    virtual void fukiTrans(f32, f32);
    virtual void fontAlpha(f32);

    /* 0XC4 */ u8 field_0xc4[0xD0 - 0xC8];
    /* 0xCC */ f32 field_0xcc;
    /* 0xD0 */ f32 field_0xd0;
};

#endif /* MSG_SCRN_D_MSG_SCRN_STAFF_H */
