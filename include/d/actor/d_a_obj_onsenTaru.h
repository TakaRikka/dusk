#ifndef D_A_OBJ_ONSENTARU_H
#define D_A_OBJ_ONSENTARU_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daOnsTaru_c
 * @brief Hotspring Water Barrel
 *
 * @details
 *
 */
class daOnsTaru_c : public fopAc_ac_c {
public:
    enum MODE_e {
        MODE_WAIT_e,
        MODE_CARRY_e,
        MODE_DROP_e,
        MODE_BREAK_e,
        MODE_SINK_e,
    };

    void setBaseMtx();
    int create();
    static int createHeapCallBack(fopAc_ac_c*);
    BOOL chkSinkAll();
    BOOL chkWaterLineIn();
    void bgCheck();
    void mode_proc_call();
    void mode_init_wait();
    void mode_proc_wait();
    void mode_init_carry();
    void mode_proc_carry();
    void mode_init_drop();
    void mode_proc_drop();
    void mode_init_break();
    void mode_proc_break();
    void mode_init_sink();
    void mode_proc_sink();
    void breakEffSet();

    virtual int CreateHeap();
    virtual int Execute();
    virtual int Draw();
    virtual int Delete();

    int getSw() { return fopAcM_GetParamBit(this, 0, 8); }
    int getSw2() { return fopAcM_GetParamBit(this, 8, 8); }
    bool getTempStat() { return mTempStat; }
    void startTimer() { mStartTimer = true; }
    
    static dCcD_SrcGObjInf const mCcDObjInfo;
    static dCcD_SrcCyl mCcDCyl;

    /* 0x56C */ request_of_phase_process_class mPhase;
    /* 0x574 */ J3DModel* mpModel;
    /* 0x578 */ u8 mMode;
    /* 0x579 */ bool mIsCarryNow;
    /* 0x57C */ cXyz mGrabOffset;
    /* 0x588 */ cXyz mPrevPos;
    /* 0x594 */ cXyz field_0x594;
    /* 0x5A0 */ u8 mInWater;
    /* 0x5A4 */ dBgS_ObjAcch mAcch;
    /* 0x77C */ dBgS_AcchCir mAcchCir;
    /* 0x7BC */ dCcD_Stts mCcStts;
    /* 0x7F8 */ dCcD_Cyl mCcCyl;
    /* 0x934 */ s16 mTimer;
    /* 0x936 */ bool mStartTimer;
    /* 0x938 */ u32 mHamonEmtrID;
    /* 0x93C */ u8 field_0x93c;
    /* 0x93D */ u8 mTempStat;
    /* 0x93E */ u8 field_0x93e;
    /* 0x93F */ u8 field_0x93F[0x944 - 0x93F];
    /* 0x944 */ f32 mCollisionRadius;
};

STATIC_ASSERT(sizeof(daOnsTaru_c) == 0x948);

class daOnsTaru_HIO_c : public fOpAcm_HIO_entry_c {
public:
    daOnsTaru_HIO_c();
    virtual ~daOnsTaru_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s16 mCoolTime;
    /* 0x06 */ u8 mVibration;
    /* 0x08 */ f32 mSmokeScale;
    /* 0x0C */ f32 mCollisionRadius;
};


#endif /* D_A_OBJ_ONSENTARU_H */
