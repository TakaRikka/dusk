#ifndef D_A_OBJ_FAN_H
#define D_A_OBJ_FAN_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjFan_c
 * @brief (City in the Sky?) Fan)
 *
 * @details
 *
 */
class daObjFan_c : public dBgS_MoveBgActor {
public:
    fopAc_ac_c* search_tornado();
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create1st();
    int Execute(Mtx**);
    void action();
    void setCollision();
    int Draw();
    int Delete();

    u8 getType() { return fopAcM_GetParamBit(this,0,4); }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mModel;
    /* 0x5AC */ dBgW* field_0x5ac;
    /* 0x5B0 */ dCcD_Stts field_0x5b0;
    /* 0x5EC */ dCcD_Sph field_0x5ec[4];
    /* 0xACC */ s16 field_0xacc;
    /* 0xACE */ s16 field_0xace;
    /* 0xAD0 */ u32 field_0xad0;
    /* 0xAD4 */ u8 field_0xad4;
    /* 0xAD8 */ cXyz field_0xad8;
};

STATIC_ASSERT(sizeof(daObjFan_c) == 0xAE4);

#endif /* D_A_OBJ_FAN_H */
