#pragma once
#include "mods/svc/actor.h"

// Base actor class definitions
#include "f_op/f_op_actor.h"

// Definitions for request_of_phase_process_class and cPhs_Step
#include "SSystem/SComponent/c_phase.h"

// Definitions for collision
#include "d/d_bg_s_acch.h"
#include "d/d_bg_w.h"
#include "d/d_cc_d.h"

#define MA_MINE_NAME "m_mine"

class ma_Mine_c : public fopAc_ac_c {
public:
    request_of_phase_process_class mPhase;
    J3DModel* mpModel;
    dBgS_ObjAcch mAcch;
    dBgS_AcchCir mAcchCir;
    Mtx mColliderMtx;
    dCcD_Stts mCcStts;
    dCcD_Sph mCollisionSphere;
    Z2SoundObjSimple mSound;
    s8 mReverb;

    virtual ~ma_Mine_c();
    cPhs_Step create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    void atHit(dCcD_GObjInf* i_atObjInf);
    static int createHeapCallBack(fopAc_ac_c*);
    static void atHitCallback(fopAc_ac_c* i_tgActor, dCcD_GObjInf* i_tgObjInf,
        fopAc_ac_c* i_atActor, dCcD_GObjInf* i_atObjInf);

    static s16 sProcName;
    static ActorHandle sActorHandle;
    static const ActorProfileDesc sProfile;
};
