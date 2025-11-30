#ifndef D_A_OBJ_VOLCBALL_H
#define D_A_OBJ_VOLCBALL_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_event_lib.h"
#include "d/d_cc_d.h"
#include "d/d_bg_s_acch.h"
#include "d/d_particle.h"

/**
 * @ingroup actors-objects
 * @class daObjVolcBall_c
 * @brief Volcano Ball
 *
 * @details
 *
 */
class daObjVolcBall_c : public fopAc_ac_c, public dEvLib_callback_c {
public:
    enum mode_e {
        /* 0 */ MODE_PLAYER_WAIT,
        /* 1 */ MODE_SWITCH_WAIT,
        /* 2 */ MODE_STOP,
        /* 3 */ MODE_QUAKE,
        /* 4 */ MODE_ERUPT,
        /* 5 */ MODE_END,
    };

    struct vball_s {
        /* 0x000 */ J3DModel* mpModel;
        /* 0x004 */ cXyz field_0x4;
        /* 0x010 */ cXyz field_0x10;
        /* 0x01C */ cXyz field_0x1c;
        /* 0x028 */ cXyz field_0x28;
        /* 0x034 */ csXyz field_0x34;
        /* 0x03C */ cXyz field_0x3c;
        /* 0x048 */ f32 field_0x48;
        /* 0x04C */ f32 field_0x4c;
        /* 0x050 */ f32 field_0x50;
        /* 0x054 */ dBgS_ObjAcch mAcch;
        /* 0x22C */ dCcD_Sph mSphCc;
        /* 0x364 */ cXyz field_0x364;
        /* 0x370 */ u32 field_0x370;
        /* 0x374 */ s16 field_0x374;
        /* 0x376 */ u8 field_0x376;
        /* 0x378 */ csXyz field_0x378;
        /* 0x37E */ u8 field_0x37E[0x0390 - 0x037E];
        /* 0x390 */ dPa_followEcallBack field_0x390[4];
    };

    class volc_param_c {
    public:
        /* 0x00 */ f32 mGravity;
        /* 0x04 */ f32 mMaxFallSpeed;
        /* 0x08 */ f32 mBallSpeedH;
        /* 0x0C */ f32 mBallSetShortRange;
        /* 0x10 */ f32 mBallSetRandom;
        /* 0x14 */ f32 mNormalScale;
        /* 0x18 */ f32 mScaleRandValue;
        /* 0x1C */ f32 mNormalInterval;
        /* 0x20 */ f32 mRandInterval;
        /* 0x24 */ f32 mNormalWaitTime;
        /* 0x28 */ f32 mRandWaitTime;
        /* 0x2C */ f32 mNormalWarningTime;
        /* 0x30 */ f32 mRandWarningTime;
        /* 0x34 */ f32 mNormalEruptTime;
        /* 0x38 */ f32 mRandEruptTime;
        /* 0x3C */ f32 mSearchBallFreq;
        /* 0x40 */ f32 mSearchBallBaseScale;
        /* 0x44 */ f32 mSearchBallRandScale;
        /* 0x48 */ f32 mSearchBallSetHeight;
        /* 0x4C */ f32 mSearchBallShadowIntensity;
        /* 0x50 */ u8 mSearchBallCreateTime;
        /* 0x51 */ u8 mPlayerSearchTime;
    };

    daObjVolcBall_c() : dEvLib_callback_c(this) {}

    volc_param_c* getData();
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create();
    int execute();
    void action();
    void initActionPlayerWait();
    void initActionWarning();
    void initActionSwWait();
    void actionPlayerWait();
    void actionSwWait();
    void actionStop();
    void actionWarning();
    void actionEruption();
    void initActionEnd();
    void actionEnd();
    void setEruptEffect(int);
    void setFallEffect(int);
    void endFallEffect(int);
    bool createSearchBall();
    bool createNormalBall();
    int executeBall();
    void startQuake();
    int draw();
    int _delete();

    virtual bool eventStart();

    u8 getSwBit() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getSwBit2() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 checkBigVolc() { return fopAcM_GetParamBit(this, 0x1F, 1); }
    u8 getEvNo() { return fopAcM_GetParamBit(this, 0x10, 8); }

    void setAction(u8 i_action) { mMode = i_action; }

private:
    /* 0x057C */ request_of_phase_process_class mPhase;
    /* 0x0584 */ dBgS_AcchCir mAcchCir;
    /* 0x05C4 */ dCcD_Stts mCcStts;
    /* 0x0600 */ u8 mIsBigVolc;
    /* 0x0601 */ u8 mMode;
    /* 0x0602 */ u8 mNormalBallCreateTimer;
    /* 0x0603 */ u8 field_0x603;
    /* 0x0604 */ u32 mTime;
    /* 0x0608 */ u32 mSearchBallCreateTimer;
    /* 0x060C */ vball_s mBall[9];
    /* 0x28EC */ Mtx field_0x28ec;
    /* 0x291C */ f32 field_0x291c;
    /* 0x2920 */ u32 mBigVolcActorID;
    /* 0x2924 */ u8 field_0x2924;
    /* 0x2925 */ u8 field_0x2925;
    /* 0x2926 */ u8 field_0x2926;
};

STATIC_ASSERT(sizeof(daObjVolcBall_c) == 0x2928);

#endif /* D_A_OBJ_VOLCBALL_H */
