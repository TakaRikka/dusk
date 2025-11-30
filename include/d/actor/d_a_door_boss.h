#ifndef D_A_DOOR_BOSS_H
#define D_A_DOOR_BOSS_H

#include "SSystem/SComponent/c_phase.h"
#include "f_op/f_op_actor.h"
#include "d/d_bg_s_acch.h"
#include "d/d_bg_w.h"

/**
 * @ingroup actors-doors
 * @class daBdoor_c
 * @brief Boss Door (Unused?)
 * 
 * @details The door that leads to the boss room.
 * 
 */
class daBdoor_c : public fopAc_ac_c {
public:
    enum Action {
        /* 0 */ ACT_WAIT,
        /* 1 */ ACT_CLOSE_WAIT,
        /* 2 */ ACT_OPEN,
        /* 3 */ ACT_END,
    };

    char* getArcName();
    char* getBmd();
    char* getDzb();
    char* getBmd2();
    J3DModelData* getDoorModelData();
    int CreateHeap();
    void calcMtx();
    int CreateInit();
    cPhs__Step create();
    int getDemoAction();
    void demoProc();
    void calcGoal(cXyz*, int);
    void smokeInit();
    BOOL checkArea();
    BOOL checkFront();
    BOOL checkOpen();
    BOOL actionWait();
    BOOL actionCloseWait();
    BOOL actionOpen();
    BOOL actionEnd();
    int execute();
    BOOL checkDraw();
    int draw();
    int Delete();

    void setAction(u8 i_action) { mAction = i_action; }

private:
    /* 0x568 */ request_of_phase_process_class mPhaseReq;
    /* 0x570 */ J3DModel* mpDoorModel;
    /* 0x574 */ J3DModel* mpLockModel;
    /* 0x578 */ mDoExt_bckAnm* mpLockAnm;
    /* 0x57C */ dBgW* mpBgW;
    /* 0x580 */ bool mBgInactive;
    /* 0x581 */ u8 mAction;
    /* 0x582 */ u8 field_0x582[6];
    /* 0x588 */ u32 mStaffID;
    /* 0x58C */ s16 field_0x58c;
    /* 0x590 */ f32 mOffsetY;
    /* 0x594 */ bool mLocked;
    /* 0x598 */ cXyz mParticlePos;
    /* 0x5A4 */ csXyz mParticleAngle;
    /* 0x5AC */ dBgS_ObjAcch mAcch;
    /* 0x784 */ dBgS_AcchCir mAcchCir;
};

STATIC_ASSERT(sizeof(daBdoor_c) == 0x7C4);

#endif /* D_A_DOOR_BOSS_H */
