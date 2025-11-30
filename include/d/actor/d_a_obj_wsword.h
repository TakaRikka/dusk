#ifndef D_A_OBJ_WSWORD_H
#define D_A_OBJ_WSWORD_H

#include "SSystem/SComponent/c_phase.h"
#include "f_op/f_op_actor.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daObjWSword_c
 * @brief Wooden Sword
 *
 * @details
 *
 */
class daObjWSword_c : public fopAc_ac_c {
public:
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create();
    int execute();
    int draw();
    int _delete();

private:
    /* 0x568 */ request_of_phase_process_class mPhaseReq;
    /* 0x570 */ J3DModel* mpModel;
    /* 0x574 */ dBgS_ObjAcch mAcch;
    /* 0x74C */ dBgS_AcchCir mAcchCir;
    /* 0x78C */ dCcD_Stts mStts;
    /* 0x7C8 */ dCcD_Cyl mCyl;
};

STATIC_ASSERT(sizeof(daObjWSword_c) == 0x904);

#endif /* D_A_OBJ_WSWORD_H */
