#ifndef D_A_OBJ_LV6BEMOS_H
#define D_A_OBJ_LV6BEMOS_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjL6Bm_c
 * @brief Temple of Time Beamos (Unused)
 *
 * @details
 *
 */
class daObjL6Bm_c : public dBgS_MoveBgActor {
public:
    typedef void (daObjL6Bm_c::*actionFunc)();

    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    cPhs__Step create1st();
    int Execute(Mtx**);
    void action();
    void actionWait();
    void actionFindPlayer();
    void actionAttack();
    int Draw();
    int Delete();

    void setAction(u8 action) { mAction = action; }

    u8 getSwBit() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getArg0() { return fopAcM_GetParamBit(this, 0x10, 8); }
    u8 getArg1() { return fopAcM_GetParamBit(this, 0x18, 8); }

    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mBeamosModel;
    /* 0x5AC */ dCcD_Stts mStts;
    /* 0x5E8 */ dCcD_Cps field_0x5e8;
    /* 0x72C */ cM3dGCpsS field_0x72c;
    /* 0x748 */ dCcD_Sph field_0x748;
    /* 0x880 */ J3DModel* mBeamEffectModel;
    /* 0x884 */ mDoExt_btkAnm* mpBtkAnm;
    /* 0x888 */ mDoExt_btkAnm* mpBtkAnm2;
    /* 0x88C */ mDoExt_bckAnm* mBeamBck;
    /* 0x890 */ u8 field_0x890;
    /* 0x891 */ u8 field_0x891;
    /* 0x892 */ s16 field_0x892;
    /* 0x894 */ cXyz field_0x894;
    /* 0x8A0 */ u8 field_0x8a0[0x8b2 - 0x8a0];
    /* 0x8B2 */ s16 field_0x8b2;
    /* 0x8B4 */ s16 field_0x8b4;
    /* 0x8B8 */ dBgS_ObjGndChk mObjGndChk;
    /* 0x90C */ f32 field_0x90c;
    /* 0x910 */ u8 mAction;
    /* 0x911 */ u8 field_0x911;
    /* 0x914 */ f32 field_0x914;
};

STATIC_ASSERT(sizeof(daObjL6Bm_c) == 0x918);


#endif /* D_A_OBJ_LV6BEMOS_H */
