#ifndef D_A_TAG_MWAIT_H
#define D_A_TAG_MWAIT_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_msg_flow.h"

class daTagMwait_c : public fopAc_ac_c {
public:
    int create();
    ~daTagMwait_c();
    int execute();

    BOOL checkEndMessage() {return fopAcM_isSwitch(this, mEnterSw); }

    /* 0x568 */ u8 mOnSw;
    /* 0x569 */ u8 mEnterSw;
    /* 0x56A */ bool mInitMsgFlow;
    /* 0x56B */ u8 mWarpToPos;
    /* 0x56C */ bool mEnteredTrigger;
    /* 0x56D */ u8 field_0x56d;
    /* 0x570 */ f32 field_0x570;
    /* 0x574 */ cXyz mWaitPosition;
    /* 0x580 */ dMsgFlow_c mMsgFlow;
};

#endif /* D_A_TAG_MWAIT_H */
