#ifndef D_A_OBJ_FALLOBJ_H
#define D_A_OBJ_FALLOBJ_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjFallObj_c
 * @brief Fall Object?
 *
 * @details
 *
 */
class daObjFallObj_c : public dBgS_MoveBgActor {
public:
    enum Action {
        ACTION_WAIT,
        ACTION_COUNTDOWN,
        ACTION_FALL,
        ACTION_FALL_START,
        ACTION_END,
        ACTION_FOLLOW_FALL,
    };

    void initBaseMtx();
    void setBaseMtx();
    int Create();
    s16 getFallTime();
    f32 getFallHeight();
    int CreateHeap();
    int create1st();
    int Execute(Mtx**);
    void action();
    bool action_wait(bool);
    bool action_fallStart();
    void action_countdown();
    void action_fall();
    void action_end();
    void action_follow_fall();
    bool checkHang();
    bool checkHang2();
    int Draw();
    int Delete();

    u32 getTime() { return fopAcM_GetParamBit(this, 8, 8); }
    u32 getPos() { return fopAcM_GetParamBit(this, 0x14, 8); }
    static BOOL checkFallStart(fopAc_ac_c* actor) { return actor->speed.y != 0.0f; }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhaseReq;
    /* 0x5A8 */ Mtx mMtx;
    /* 0x5D8 */ u8 field_0x5d8;
    /* 0x5D9 */ u8 mTimer;
    /* 0x5DA */ s16 mFallTime;
    /* 0x5DC */ s16 field_0x5dc;
    /* 0x5DE */ s16 field_0x5de;
    /* 0x5E0 */ int field_0x5e0;
    /* 0x5E4 */ bool field_0x5e4;
    /* 0x5E5 */ bool field_0x5e5;
    /* 0x5E8 */ J3DModel* mpModel;
    /* 0x5EC */ fpc_ProcID mChildId;
    /* 0x5F0 */ u8 mAction;
    /* 0x5F1 */ bool mVibrationOn;
    /* 0x5F4 */ Z2SoundObjSimple mSoundObj;
};

STATIC_ASSERT(sizeof(daObjFallObj_c) == 0x614);

#if DEBUG
class daObjFallObj_HIO_c : public mDoHIO_entry_c {
public:
    daObjFallObj_HIO_c();
    void genMessage(JORMContext* ctx);

    /* 0x6 */ s8 mId;
    /* 0x8 */ s16 mMovementShake;
    /* 0xA */ s16 mStaticShake;
    /* 0xC */ u8 mNoFall;
    /* 0xD */ u8 mCheckDisplay;
};
#endif


#endif /* D_A_OBJ_FALLOBJ_H */
