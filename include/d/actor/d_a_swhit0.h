#ifndef D_A_SWHIT0_H
#define D_A_SWHIT0_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-unsorted
 * @class daSwhit0_c
 * @brief Crystal Switch (?)
 *
 * @details
 *
 */
class daSwhit0_c : public fopAc_ac_c, public request_of_phase_process_class {
public:
    enum Action_e {
        ACTION_SW_WAIT_e,
        ACTION_OFF_WAIT_e,
        ACTION_TO_ON_READY_e,
        ACTION_TO_ON_ORDER_e,
        ACTION_TO_ON_DEMO_e,
        ACTION_TO_ON_DEMO2_e,
        ACTION_ON_WAIT_e,
        ACTION_TOOL_TO_ON_READY_e,
        ACTION_TOOL_TO_ON_ORDER_e,
        ACTION_TOOL_TO_ON_DEMO_e,
        ACTION_TOOL_TO_ON_DEMO2_e,
    };

    int getSwNo();
    int getSwNo2();
    u8 getEvNo();
    u8 getTimer();
    u8 getType();
    void makeEventId();
    int CreateHeap();
    int CreateInit();
    int create();
    bool checkHit();
    void setCombackTimer();
    int endCombackTimer();
    void onSwitch();
    void offSwitch();
    int DemoProc();
    void orderEvent();
    int actionOffWait();
    int actionToOnReady();
    int actionToOnOrder();
    int actionToOnDemo();
    int actionToOnDemo2();
    int actionSwWait();
    int actionOnWait();
    void setDrawMtx();

    inline int draw();
    inline int execute();

    void setActio(u8 i_action) { mAction = i_action; }
    void onFlag(u16 i_flag) { mFlags |= i_flag; }
    void offFlag(u16 i_flag) { mFlags &= ~i_flag; }
    bool checkFlag(u16 i_flag) { return mFlags & i_flag; }

    /* 0x570 */ J3DModel* mpModel;
    /* 0x574 */ mDoExt_bckAnm mBck;
    /* 0x590 */ mDoExt_brkAnm mBrk;
    /* 0x5A8 */ Mtx field_0x5a8;
    /* 0x5D8 */ dCcD_Stts mColliderStts;
    /* 0x614 */ dCcD_Sph mSph;
    /* 0x74C */ u8 mHitWaitTimer;
    /* 0x74D */ u8 mAction;
    /* 0x74E */ u8 mReadyTimer;
    /* 0x750 */ s16 mCombackTimer;
    /* 0x752 */ s16 mSwWaitTimer;
    /* 0x754 */ u16 mFlags;
    /* 0x756 */ s16 mEventId;
    /* 0x758 */ int mStaffId;
};

STATIC_ASSERT(sizeof(daSwhit0_c) == 0x75c);


#endif /* D_A_SWHIT0_H */
