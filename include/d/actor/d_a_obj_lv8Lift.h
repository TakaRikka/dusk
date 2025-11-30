#ifndef D_A_OBJ_LV8LIFT_H
#define D_A_OBJ_LV8LIFT_H

#include "d/d_bg_s_acch.h"
#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daL8Lift_c
 * @brief Palace of Twilight Platform Lift
 *
 * @details
 *
 */
class daL8Lift_c : public dBgS_MoveBgActor {
public:
    enum MODE_e {
        MODE_ACC_e,
        MODE_MOVE_e,
        MODE_BRK_e,
        MODE_WAIT_e,
        MODE_WAIT_INIT_e,
        MODE_ON_ANM_e,
        MODE_MOVE_WAIT_e,
        MODE_STOP_e,
        MODE_INIT_SET_e,
        MODE_INIT_SET2_e
    };

    void setBaseMtx();
    int CreateHeap();
    int create();
    void lightSet();
    static void rideCallBack(dBgW*, fopAc_ac_c*, fopAc_ac_c*);
    int Execute(Mtx**);
    void moveLift();
    void modeAcc();
    void init_modeMove();
    void modeMove();
    void init_modeBrk();
    void modeBrk();
    void init_modeWaitInit();
    void modeWaitInit();
    void init_modeWait();
    void modeWait();
    void init_modeMoveWait();
    void modeMoveWait();
    void init_modeOnAnm();
    void modeOnAnm();
    void init_modeStop();
    void modeStop();
    void init_modeInitSet();
    void modeInitSet();
    void init_modeInitSet2();
    void modeInitSet2();
    void liftReset();
    void setNextPoint();
    int Draw();
    int Delete();

    u8 getPthID() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getMoveSpeed() { return fopAcM_GetParamBit(this, 8, 4); }
    int getSw() { return fopAcM_GetParamBit(this, 12, 8); }

    static f32 const mSpeed[16];

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ mDoExt_btkAnm mBtk;
    /* 0x5C4 */ u8 field_0x5c4;
    /* 0x5C5 */ u8 field_0x5c5;
    /* 0x5C6 */ u8 mPathID;
    /* 0x5C7 */ s8 mPathDirection;
    /* 0x5C8 */ s16 mCurrentPoint;
    /* 0x5CA */ u8 field_0x5ca;
    /* 0x5CB */ u8 field_0x5cb;
    /* 0x5CC */ cXyz mTargetPos;
    /* 0x5D8 */ cXyz mPrevTargetPos;
    /* 0x5E4 */ f32 mMoveSpeed;
    /* 0x5E8 */ u8 mMode;
    /* 0x5E9 */ u8 mSwbit;
    /* 0x5EA */ u8 mIsSwitch;
    /* 0x5EB */ u8 field_0x5eb;
    /* 0x5EC */ dBgS_AcchCir mAcchCir;
    /* 0x62C */ dBgS_ObjAcch mAcch;
    /* 0x804 */ u8 field_0x804;
    /* 0x805 */ u8 field_0x805;
    /* 0x806 */ u8 field_0x806;
    /* 0x807 */ u8 field_0x807;
    /* 0x808 */ u8 field_0x808;
    /* 0x809 */ u8 mStopDisappearTimer;
    /* 0x80A */ u8 mPlayerRide;
    /* 0x80B */ u8 mPrevPlayerRide;
    /* 0x80C */ u8 mNoRideOffSwTimer;
    /* 0x80D */ u8 mLightSet;
    /* 0x80E */ u8 field_0x80e;
    /* 0x80F */ u8 field_0x80f;
    /* 0x810 */ LIGHT_INFLUENCE mLight;
};

STATIC_ASSERT(sizeof(daL8Lift_c) == 0x830);

class daL8Lift_HIO_c : public mDoHIO_entry_c {
public:
    daL8Lift_HIO_c();
    virtual ~daL8Lift_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ u8 mStopDisappearTime;
    /* 0x05 */ u8 mStartMoveTime;
    /* 0x06 */ u8 mColorAnmFrame;
    /* 0x07 */ u8 mColorR;
    /* 0x08 */ u8 mColorG;
    /* 0x09 */ u8 mColorB;
    /* 0x0C */ f32 mLightRadius;
    /* 0x10 */ u8 mLightColorR;
    /* 0x11 */ u8 mLightColorG;
    /* 0x12 */ u8 mLightColorB;
};


#endif /* D_A_OBJ_LV8LIFT_H */
