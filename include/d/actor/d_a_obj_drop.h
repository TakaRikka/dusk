#ifndef D_A_OBJ_DROP_H
#define D_A_OBJ_DROP_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_particle.h"
#include "d/d_cc_d.h"
#include "d/d_bg_s_acch.h"

/**
 * @ingroup actors-objects
 * @class daObjDrop_c
 * @brief Tear of Light
 *
 * @details
 *
 */
class daObjDrop_c : public fopAc_ac_c {
public:
    enum MODE_e {
        MODE_INIT_e,
        MODE_PARENT_WAIT_e,
        MODE_NO_PARENT_e,
        MODE_WAIT_e,
    };

    enum DEMOMODE_e {
        DEMOMODE_COMPLETE_WAIT_e,
        DEMOMODE_ORDER_COMPLETE_DEMO_e,
        DEMOMODE_COMPLETE_DEMO_e,
        DEMOMODE_WAIT_COMPLETE_GET_DEMO_e,
        DEMOMODE_COMPLETE_GET_DEMO_e,
    };

    int Create();
    int create();
    void dropGet();
    BOOL checkGetArea();
    void checkCompleteDemo();
    void createLineEffect();
    void removeLineEffect();
    void createBodyEffect();
    void removeBodyEffect();
    int modeInit();
    int modeNoParent();
    int modeParentWait();
    int modeWait();
    int actionCompleteWait();
    int actionOrderCompleteDemo();
    int actionCompleateDemo();
    int actionWaitCompleteGetDemo();
    int actionCompleteGetDemo();
    int execute();
    int _delete();

    u8 getYmSwbit() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 getSave() { return fopAcM_GetParamBit(this, 0, 6); }

    void setPos(cXyz i_pos) { current.pos = i_pos; }
    void setDemoMode(u8 i_mode) { mDemoMode = i_mode; }
    void setMode(u8 i_mode) { mMode = i_mode; }
    void setAppear() { mAppear = 1; }
    u8 chkDemoMode() { return mDemoMode; }

    /* 0x568 */ cXyz mLinePos[3];
    /* 0x58C */ cXyz mLineMovePos[3];
    /* 0x5B0 */ csXyz mTargetAngle[3];
    /* 0x5C2 */ s16 mTargetRelAngle[3];
    /* 0x5C8 */ u8 mLineIsCross[3];
    /* 0x5CC */ JPABaseEmitter* mpLineEffEmtrs[3];
    /* 0x5D8 */ dPa_followEcallBack mLineEffCallbacks[3];
    /* 0x614 */ JPABaseEmitter* mpBodyEffEmtrs[6];
    /* 0x62C */ dPa_followEcallBack mBodyEffCallbacks[6];
    /* 0x6A4 */ int mCounter;
    /* 0x6A8 */ fpc_ProcID mPtI_Id;
    /* 0x6AC */ u32 field_0x6ac;
    /* 0x6B0 */ u8 mMode;
    /* 0x6B1 */ s8 mModeTimer;
    /* 0x6B2 */ u8 mModeAction;
    /* 0x6B3 */ u8 field_0x6B3[0x6B5 - 0x6B3];
    /* 0x6B5 */ u8 mDeleteTimer;
    /* 0x6B6 */ u8 mDrawInTimer;
    /* 0x6B7 */ u8 field_0x6b7;
    /* 0x6B8 */ u8 mAppear;
    /* 0x6B9 */ u8 mSetCollectDrop;
    /* 0x6BA */ u8 mDemoMode;
    /* 0x6BB */ u8 mFadeoutTimer;
    /* 0x6BC */ u8 field_0x6bc;
    /* 0x6C0 */ Z2SoundObjSimple mSound;
    /* 0x6E0 */ dBgS_AcchCir mAcchCir;
    /* 0x720 */ dBgS_ObjAcch mAcch;
    /* 0x8F8 */ dCcD_Stts mCcStts;
    /* 0x934 */ dCcD_Sph mCcSph;
    /* 0xA6C */ cXyz mTargetPos;
    /* 0xA78 */ u8 field_0xa78[0xA7C - 0xA78];
};

STATIC_ASSERT(sizeof(daObjDrop_c) == 0xa7c);

class daObjDrop_HIO_c {
public:
    u8 pad[0x17];
    /* 0x17 */ u8 field_0x17;
};

#endif /* D_A_OBJ_DROP_H */
