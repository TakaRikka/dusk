#ifndef D_A_OBJ_LV4CHANDELIER_H
#define D_A_OBJ_LV4CHANDELIER_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_model.h"

/**
 * @ingroup actors-objects
 * @class daObjLv4Chan_c
 * @brief Arbiter's Grounds Chandelier
 *
 * @details
 *
 */
class daObjLv4Chan_c : public dBgS_MoveBgActor, public request_of_phase_process_class {
public:
    struct ChainPos {
        /* 0x00 */ cXyz field_0x0;
        /* 0x0C */ cXyz field_0xc;
        /* 0x18 */ cXyz field_0x18;
        /* 0x24 */ f32 field_0x24;
    };

    int create1st();
    void setMtxChain(daObjLv4Chan_c::ChainPos*, dMdl_obj_c*, int);
    void setMtx();
    void rideActor(fopAc_ac_c*);
    int CreateHeap();
    int Create();
    void constraintChain(daObjLv4Chan_c::ChainPos*, int);
    void constraintBase();
    void calcVec(daObjLv4Chan_c::ChainPos*, int);
    void chkGnd();
    int Execute(Mtx**);
    int Draw();
    int Delete();

    int getArg0() { return fopAcM_GetParamBit(this, 0, 4); }
    int getAddChain0() { return fopAcM_GetParamBit(this, 4, 4); }
    int getAddChain1() { return fopAcM_GetParamBit(this, 8, 4); }
    int getAddChain2() { return fopAcM_GetParamBit(this, 12, 4); }
    int getSw() { return fopAcM_GetParamBit(this, 16, 8); }

private:
    /* 0x05A8 */ Mtx field_0x5a8;
    /* 0x05D8 */ Mtx field_0x5d8;
    /* 0x0608 */ J3DModel* mModel;
    /* 0x060C */ J3DModelData* mChainModelData;
    /* 0x0610 */ int mChainLengths[3];
    /* 0x061C */ ChainPos* mChains[3];
    /* 0x0628 */ int mMdlLengths[3];
    /* 0x0634 */ dMdl_obj_c* mMdls[3];
    /* 0x0640 */ dCcD_Stts mStts;
    /* 0x067C */ dCcD_Sph mSphs1[8];
    /* 0x103C */ dCcD_Sph mSphs2[8];
    /* 0x19FC */ dCcD_Sph mSphs3[8];
    /* 0x23BC */ cXyz field_0x23bc;
    /* 0x23C8 */ cXyz field_0x23c8;
    /* 0x23D4 */ int field_0x23d4;
    /* 0x23D8 */ fopAc_ac_c* field_0x23d8[8];
    /* 0x23F8 */ fopAc_ac_c* field_0x23f8[8];
    /* 0x2418 */ int field_0x2418;
    /* 0x241C */ int field_0x241c;
    /* 0x2420 */ f32 field_0x2420;
    /* 0x2424 */ int mShadowId;
    /* 0x2428 */ dBgS_ObjGndChk mGndChk;
    /* 0x247C */ f32 field_0x247c;
    /* 0x2480 */ int mAddChain0;
    /* 0x2484 */ int field_0x2484;
    /* 0x2488 */ u8 field_0x2488;
    /* 0x2489 */ u8 field_0x2489;
    /* 0x248A */ u8 field_0x248a;
    /* 0x248B */ u8 field_0x248b;
    /* 0x248C */ u8 field_0x248c;
    /* 0x248D */ u8 field_0x248d;
    /* 0x2490 */ f32 field_0x2490;
    /* 0x2494 */ f32 field_0x2494;
    /* 0x2498 */ u8 field_0x2498;
    /* 0x2499 */ u8 field_0x2499;
    /* 0x249C */ f32 field_0x249c;
    /* 0x24A0 */ f32 field_0x24a0;
};

STATIC_ASSERT(sizeof(daObjLv4Chan_c) == 0x24a4);

#endif /* D_A_OBJ_LV4CHANDELIER_H */
