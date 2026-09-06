#pragma once
#include "mods/svc/actor.h"

// Base actor class definitions
#include "f_op/f_op_actor.h"

// Definitions for request_of_phase_process_class and cPhs_Step
#include "SSystem/SComponent/c_phase.h"

// Definitions for collision
#include "d/d_bg_s_acch.h"
#include "d/d_bg_w.h"

#define MAOBJ_WROCK_NAME "wrock"

class maObj_Wrock_c : public fopAc_ac_c {
public:
    request_of_phase_process_class mPhase;
    J3DModel* mpModel;
    dBgS_ObjAcch mAcch;
    cBgS_GndChk mGndChk;
    dBgS_AcchCir mAcchCir;
    Mtx mColliderMtx;
    dBgW* mpCollider;
    f32 mGroundH;
    int mShadow;

    virtual ~maObj_Wrock_c();
    cPhs_Step create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);

    static s16 sProcName;
    static ActorHandle sActorHandle;
    static const ActorProfileDesc sProfile;
};
