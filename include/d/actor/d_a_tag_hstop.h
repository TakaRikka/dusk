#ifndef D_A_TAG_HSTOP_H
#define D_A_TAG_HSTOP_H

#include "f_op/f_op_actor.h"
#include "d/d_msg_flow.h"

class daTagHstop_c : public fopAc_ac_c {
public:
    int create();
    ~daTagHstop_c();
    void setActive();
    int execute();

    static daTagHstop_c* getTop() { return m_top; }

    daTagHstop_c* getNext() { return mNext; }
    u8 getActiveFlg() const { return mActive; }

    void onTagStop() {
        if (field_0x573 == 0) {
            field_0x573 = 1;
        }
    }

    static daTagHstop_c* m_top;
    static dMsgFlow_c m_msgFlow;

    /* 0x568 */ daTagHstop_c* mNext;
    /* 0x568 */ daTagHstop_c* mPrev;
    /* 0x570 */ u8 mPrm0;
    /* 0x571 */ u8 mActive;
    /* 0x572 */ u8 mPrm1;
    /* 0x573 */ u8 field_0x573;
    /* 0x574 */ int field_0x574;
};

#endif /* D_A_TAG_HSTOP_H */
