#ifndef D_A_OBJ_BURNBOX_H
#define D_A_OBJ_BURNBOX_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjBurnBox_c
 * @brief Burn Box
 *
 * @details
 *
 */
class daObjBurnBox_c : public dBgS_MoveBgActor {
public:
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    cPhs__Step create1st();
    int Execute(Mtx**);
    int Draw();
    int Delete();

    static void make_prm_burnBox(u32* param_0, u8 param_1) { *param_0 = param_1; }

    u8 getType() { return fopAcM_GetParamBit(this, 0, 8); }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhaseReq;
    /* 0x5A8 */ J3DModel* mpBoxModel;
    /* 0x5AC */ dCcD_Stts mStts;
    /* 0x5E8 */ dCcD_Cyl mCyl;
    /* 0x724 */ u32 field_0x724;
    /* 0x728 */ s16 field_0x728;
    /* 0x72a */ u8 field_0x72a;
    /* 0x72b */ u8 field_0x72b;
    /* 0x72c */ JPABaseEmitter* mpEmitters[5];
};

STATIC_ASSERT(sizeof(daObjBurnBox_c) == 0x740);

#endif /* D_A_OBJ_BURNBOX_H */
