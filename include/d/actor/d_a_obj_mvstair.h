#ifndef D_A_OBJ_MVSTAIR_H
#define D_A_OBJ_MVSTAIR_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_particle.h"

/**
 * @ingroup actors-objects
 * @class daObjStair_c
 * @brief Moving Stairs
 *
 * @details
 *
 */
class daObjStair_c : public dBgS_MoveBgActor {
public:
    enum Action_e {
        ACTION_WAIT_e,
        ACTION_ORDER_EVENT_e,
        ACTION_EVENT_e,
        ACTION_END_e,
    };

    enum MODE_e {
        MODE_WAIT_UPPER_e,
        MODE_UPPER_e,
        MODE_WAIT_LOWER_e,
        MODE_LOWER_e,
    };

    void initBaseMtx();
    void setBaseMtx();
    int create1st();
    void event_proc_call();
    void mode_proc_call();
    int actionWait();
    int actionOrderEvent();
    int actionEvent();
    int actionEnd();
    int modeWaitUpperInit();
    int modeWaitUpper();
    int modeUpperInit();
    int modeUpper();
    int modeWaitLowerInit();
    int modeWaitLower();
    int modeLowerInit();
    int modeLower();
    void setParticle();
    void removeParticle();

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    u8 getSwbit() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getSwbit2() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 getEvNo() { return fopAcM_GetParamBit(this, 16, 8); }
    u8 getDir() { return fopAcM_GetParamBit(this, 28, 1); }
    u8 getType() { return fopAcM_GetParamBit(this, 24, 4); }
    u8 getMdlType() { return fopAcM_GetParamBit(this, 29, 3); }

    void setAction(u8 i_action) { mAction = i_action; }

    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ JPABaseEmitter* mSmokeEmitter;
    /* 0x5B0 */ dPa_followEcallBack mParticleLeftEmitter[2];
    /* 0x5D8 */ dPa_followEcallBack mParticleRightEmitter[2];
    /* 0x600 */ cXyz mParticleLeftPos[2];
    /* 0x618 */ cXyz mParticleRightPos[2];
    /* 0x630 */ cXyz mSoundPos;
    /* 0x63C */ u8 mType;
    /* 0x63D */ u8 mAction;
    /* 0x63E */ u8 mMode;
    /* 0x63F */ u8 mIsSwitch;
    /* 0x640 */ s16 mEventId;
    /* 0x644 */ f32 mMoveAccel;
    /* 0x648 */ f32 mMoveStep;
    /* 0x64C */ u8 field_0x64C[0x650 - 0x64C];
    /* 0x650 */ cXyz mModelPos;
    /* 0x65C */ u16 mMoveStartShakeTimer;
    /* 0x65E */ s16 mMoveEndShakeTimer;
    /* 0x660 */ f32 mMoveStartShakeAmount;
    /* 0x664 */ u8 field_0x664[0x668 - 0x664];
    /* 0x668 */ u8 mMoveMode;
    /* 0x669 */ u8 mResType;
};

STATIC_ASSERT(sizeof(daObjStair_c) == 0x66c);


#endif /* D_A_OBJ_MVSTAIR_H */
