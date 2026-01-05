#ifndef D_A_TAG_KAGO_FALL_H
#define D_A_TAG_KAGO_FALL_H

#include "d/d_msg_flow.h"
#include "f_op/f_op_actor.h"

class daTagKagoFall_c : public fopAc_ac_c {
public:
    enum ActionMode {
        /* 0 */ ACTION_MODE_RIVER,
        /* 1 */ ACTION_MODE_FALL
    };

    int create();
    int execute();
    void setActionMode(u8 mode, u8 state);
    void actionWaitRiver();
    void actionWaitFall();
    int _delete();

    /* 0x568 */ dMsgFlow_c mMsgFlow;
    /* 0x5b4 */ Mtx mMtx;
    /* 0x5e4 */ cXyz mRestartPos;
    /* 0x5f0 */ s16 mTimer;
    /* 0x5f2 */ s16 mNoCarryTimer;
    /* 0x5f4 */ u8 mRiverTimer;
    /* 0x5f5 */ u8 mActionMode;
    /* 0x5f6 */ u8 mActionState;
    /* 0x5f7 */ u8 mExitID;
    /* 0x5f8 */ u8 mStartPoint;
    /* 0x5f9 */ bool mPlayedSceneChangeSfx;

};  // Size: 0x5fc

STATIC_ASSERT(sizeof(daTagKagoFall_c) == 0x5fc);

#endif /* D_A_TAG_KAGO_FALL_H */
