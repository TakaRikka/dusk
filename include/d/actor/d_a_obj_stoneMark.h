#ifndef D_A_OBJ_STONEMARK_H
#define D_A_OBJ_STONEMARK_H

#include "SSystem/SComponent/c_phase.h"
#include "f_op/f_op_actor.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daObjSMark_c
 * @brief Stone Mark
 *
 * @details
 *
 */
class daObjSMark_c : public fopAc_ac_c {
public:
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int create();
    int draw();
    int _delete();

private:
    /* 0x0568 */ request_of_phase_process_class mpPhase;
    /* 0x0570 */ GXTexObj mTexObj;
    /* 0x0590 */ dBgS_ObjAcch mObjAcch;
    /* 0x0768 */ dBgS_AcchCir mAcchCir;
    /* 0x07A8 */ dCcD_Stts mStts;
    /* 0x07E4 */ Mtx cullMtx;
};

STATIC_ASSERT(sizeof(daObjSMark_c) == 0x0814);

#endif /* D_A_OBJ_STONEMARK_H */
