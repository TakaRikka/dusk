#ifndef D_A_OBJ_WATERPILLAR_H
#define D_A_OBJ_WATERPILLAR_H

/**
 * @ingroup actors-objects
 * @class daWtPillar_c
 * @brief Water Column/Pillar
 *
 * @details Water columns that can carry broken stalactites (see d_a_obj_syrock)
 *
*/

#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_event_lib.h"
#include "f_op/f_op_actor.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_ext.h"
#include "SSystem/SComponent/c_phase.h"
class daWtPillar_c : public fopAc_ac_c, public request_of_phase_process_class, public dEvLib_callback_c {
public:
    daWtPillar_c() : dEvLib_callback_c(this) {}
    ~daWtPillar_c() {}

    void setBaseMtx();
    static int createHeapCallBack(fopAc_ac_c*);
    int CreateHeap();
    cPhs_Step create();
    int execute();
    void actionMain();
    void effectSet();
    void effectSet2();
    void actionSwWaitInit();
    void actionSwWait();
    virtual bool eventStart();
    void actionWaitInit();
    void actionWait();
    void actionUpFirstInit();
    void actionUpFirst();
    void actionUpFirstWaitInit();
    void actionUpFirstWait();
    void actionUpInit();
    void actionUp();
    void actionUpWaitInit();
    void actionUpWait();
    void actionDownInit();
    void actionDown();
    void actionRockWaitInit();
    void actionRockWait();
    void actionRockOnInit();
    void actionRockOn();
    void actionEndInit();
    void actionEnd();
    f32 getPillarHeight();
    int draw();
    int _delete();

    static dCcD_SrcGObjInf const mCcDObjInfo;
    static dCcD_SrcGObjInf const mCcDObjCoInfo;
    static dCcD_SrcCps mCcDCps;
    static dCcD_SrcCyl mCcDCyl;

    void onRockFlag() {
        mCarryingStalactite = true;
    }

    cXyz getPos() {
        return mTopPos;
    }

    u8 isRockYure() {
        return mStartStalactiteShake;
    }

    void clearRockYure() {
        mStartStalactiteShake = false;
    }

private:
    /* 0x584 */ request_of_phase_process_class mPhase;
    /* 0x58C */ J3DModel* mpModel;
    /* 0x590 */ mDoExt_btkAnm mVTexScrollAnm;
    /* 0x5A8 */ mDoExt_bckAnm mModelRotAnm;
    /* 0x5C4 */ dBgS_ObjAcch mAcch;
    /* 0x79C */ dBgS_AcchCir mAcchCir;
    /* 0x7DC */ dCcD_Stts mStts;
    /* 0x818 */ dCcD_Cps mCapsuleCollider;
    /* 0x95C */ dCcD_Cyl mCylinderCollider;
    /* 0xA98 */ u8 pad0[0x30];
    /* 0xAC8 */ cXyz field_0xAC8;   // unused.
    /* 0xAD4 */ cXyz mDbgDrawScale;
    /* 0xAE0 */ f32 mCurrentHeight;
    /* 0xAE4 */ cM3dGCpsS mCapsuleSource;
    /* 0xB00 */ u8 mAction;
    /* 0xB02 */ u16 mWaitFrames;
    /* 0xB04 */ f32 mTargetSpeed;
    /* 0xB08 */ u8 mSwitchNo;
    /* 0xB09 */ u8 mType;
    /* 0xB0A */ u8 pad2[0x6];
    /* 0xB10 */ f32 mUpFirstTargetHeight;
    /* 0xB14 */ f32 mMaxHeight;
    /* 0xB18 */ f32 mTargetHeightStalactiteOffset;
    /* 0xB1C */ f32 mRelativeWaterHeight;
    /* 0xB20 */ u32 mVOscAngleStep;
    /* 0xB24 */ cXyz mVOscVOffset;
    /* 0xB30 */ f32 mVOscAngleQuantum;
    /* 0xB34 */ f32 mVOscAmplitude;
    /* 0xB38 */ f32 mVOscDampingScale;
    /* 0xB3C */ f32 mVOscMaxDecay;
    /* 0xB40 */ f32 mVOscMinDecay;
    /* 0xB44 */ u8 field_0xB44; // Modified, but never read; unused?
    /* 0xB45 */ u8 mIsUpOrStatic;
    /* 0xB46 */ u8 mIsUpFirst;
    /* 0xB48 */ u32 mBotAndTopEmmitters[7];
    /* 0xB64 */ u32 mWaterSurfaceEmitters[2];
    /* 0xB6C */ cXyz mTopPos;
    /* 0xB78 */ u8 mStartStalactiteShake;   // Modified by d_a_obj_syRock
    /* 0xB79 */ u8 mCarryingStalactite;     // Modified by d_a_obj_syRock

    s32 getEventID() {
        return shape_angle.x & 0xFF;
    }

    s32 getParam(int shift, int bit) {
        return fopAcM_GetParamBit(this, shift, bit);
    }

    enum Action_e {
        ACTION_SW_WAIT,
        ACTION_WAIT,
        ACTION_UP_FIRST, ACTION_UP_FIRST_WAIT,
        ACTION_UP, ACTION_UP_WAIT,
        ACTION_DOWN,
        ACTION_ROCK_WAIT, ACTION_ROCK_ON,
        ACTION_END
    };

    enum Type_e {
        STATIC,
        RISES_AND_FALLS
    };
};


STATIC_ASSERT(sizeof(daWtPillar_c) == 0xb7c);

#endif /* D_A_OBJ_WATERPILLAR_H */
