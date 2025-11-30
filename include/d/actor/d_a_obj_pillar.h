#ifndef D_A_OBJ_PILLAR_H
#define D_A_OBJ_PILLAR_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daPillar_c
 * @brief Forest Temple Totem Pole
 *
 * @details
 *
 */
class daPillar_c : public dBgS_MoveBgActor {
public:
    enum shake_e {
        SHAKE_CRASH,
        SHAKE_STRONG,
        SHAKE_WEAK,
        SHAKE_CRASH_LV1,
        SHAKE_IRONBALL,
        SHAKE_NONE = 0xFF,
    };

    struct sdata_t {
        /* 0x0 */ u8 mPower;
        /* 0x1 */ u8 field_0x1;
        /* 0x2 */ s16 mInitAngleX;
        /* 0x4 */ s16 mTime;
        /* 0x6 */ s16 mSpeedY;
        /* 0x8 */ s16 mMaxChangeX;
        /* 0xA */ s16 mMinChangeX;
    };

    void setBaseMtx();
    int create1st();
    bool checkAttacked();
    void on_switch();
    int checkShake();
    void setShake_rcrash();
    void setShake_strong();
    void setShake_weak();
    void switch_proc_call();
    void actionSwOnWait();
    void actionSwOn();
    void actionEnd();
    
    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    u16 getTimer() { return fopAcM_GetParamBit(this, 0x18, 8); }
    u8 checkShadow() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 getSwbit() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getType() { return fopAcM_GetParamBit(this, 0x10, 8); }


    u8 getMdlType() { return field_0x73e & 0xF; }
    bool checkRollAttack() { return mIsRollAttack == true; }
    void setShake(int i_shake) { mShake = i_shake; }
    void setAction(u8 i_action) { mAction = i_action; }

    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ dCcD_Stts mColliderStts;
    /* 0x5E8 */ dCcD_Cyl mCylCollider;
    /* 0x724 */ J3DModel* mpShadowModel;
    /* 0x728 */ u8 field_0x728[0x72D - 0x728];
    /* 0x72D */ u8 mIsPrmInit;
    /* 0x72E */ s16 mRotY;
    /* 0x730 */ u8 field_0x730[0x732 - 0x730];
    /* 0x732 */ u8 mIsRollAttack;
    /* 0x733 */ u8 mAction;
    /* 0x734 */ s16 field_0x734;
    /* 0x736 */ s16 mTimer;
    /* 0x738 */ int mShake;
    /* 0x73C */ s16 field_0x73c;
    /* 0x73E */ u16 field_0x73e;
    /* 0x740 */ u8 mMdlType;
    /* 0x741 */ u8 mShakeMode;
    /* 0x742 */ sdata_t mShakeData;
    /* 0x74E */ s16 mQuakeTimer;
};

STATIC_ASSERT(sizeof(daPillar_c) == 0x750);

class daPillar_HIO_c : public mDoHIO_entry_c {
public:
    daPillar_HIO_c();
    virtual ~daPillar_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ daPillar_c::sdata_t mShakeData;
    /* 0x10 */ u8 field_0x10[0x1C - 0x10];
    /* 0x1C */ f32 field_0x1c;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ u8 field_0x24;
    /* 0x25 */ u8 field_0x25;
    /* 0x26 */ u8 field_0x26;
    /* 0x27 */ u8 mIsAdjust;
};


#endif /* D_A_OBJ_PILLAR_H */
