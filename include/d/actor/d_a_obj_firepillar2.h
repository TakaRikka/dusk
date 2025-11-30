#ifndef D_A_OBJ_FIREPILLAR2_H
#define D_A_OBJ_FIREPILLAR2_H

#include "JSystem/JParticle/JPAEmitter.h"
#include "SSystem/SComponent/c_phase.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjFPillar2_c
 * @brief Lava Fire Pillar
 *
 * @details
 *
 */
class daObjFPillar2_c : public fopAc_ac_c {
public:
    typedef void (daObjFPillar2_c::*actionFunc)();

    enum Kind {
        KIND_PIPE_FIRE,
        KIND_MAGMA_POLE,
    };

    enum Action {
        ACTION_OFF,
        ACTION_ON_WAIT,
        ACTION_ON,
    };

    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create();
    int execute();
    void action();
    void actionOffInit();
    void actionOff();
    void actionOnWaitInit();
    void actionOnWait();
    void actionOnInit();
    void actionOn();
    int draw();
    int _delete();

    u8 getKind() { return (field_0x97a >> 4) & 0xf; }
    u8 getSize() { return (field_0x97a) & 0xf; }
    u8 getArg0() { return fopAcM_GetParamBit(this,8,8); }
    u8 getSwNo() { return fopAcM_GetParamBit(this,0,8); }

private:
    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ J3DModel* mModel;
    /* 0x574 */ mDoExt_bckAnm* mBck;
    /* 0x578 */ mDoExt_btkAnm* mBtk;
    /* 0x57C */ dBgS_ObjAcch mAcch;
    /* 0x754 */ dBgS_AcchCir mAcchCir;
    /* 0x794 */ dCcD_Stts mStts;
    /* 0x7D0 */ dCcD_Cps mCps;
    /* 0x914 */ Mtx mMtx;
    /* 0x944 */ cXyz field_0x944;
    /* 0x950 */ cXyz field_0x950;
    /* 0x95C */ cM3dGCpsS field_0x95c;
    /* 0x978 */ u16 mActionTimer;
    /* 0x97A */ u16 field_0x97a;
    /* 0x97C */ s16 field_0x97c;
    /* 0x97E */ bool mInitAngles;
    /* 0x97F */ u8 mAction;
    /* 0x980 */ JPABaseEmitter* field_0x980;
    /* 0x984 */ JPABaseEmitter* field_0x984;
    /* 0x988 */ JPABaseEmitter* mPipeFireEmitters[3];
    /* 0x994 */ f32 mPipeFireRate;
    /* 0x998 */ f32 mPipeFireDirectionalSpeed;
    /* 0x99C */ f32 mPipeFireScale;
    /* 0x9A0 */ f32 mPipeFireLifeTime;
    /* 0x9A4 */ JPABaseEmitter* mMagmaPoleEmitters[3];
    /* 0x9B0 */ cXyz mYoganScale;
    /* 0x9BC */ Z2SoundObjSimple mSoundObj;
    /* 0x9DC */ cXyz mSoundPos;
    /* 0x9E8 */ s8 mSoundDir;
    /* 0x9E9 */ u8 mFirePipeTimer;
};

STATIC_ASSERT(sizeof(daObjFPillar2_c) == 0x9ec);


#endif /* D_A_OBJ_FIREPILLAR2_H */
