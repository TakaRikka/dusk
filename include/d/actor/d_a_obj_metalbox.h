#ifndef D_A_OBJ_METALBOX_H
#define D_A_OBJ_METALBOX_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daObjMBox_c
 * @brief Metal Box
 *
 * @details
 *
 */
class daObjMBox_c : public dBgS_MoveBgActor {
private:
    /* 0x5a0 */ request_of_phase_process_class mPhase;
    /* 0x5a8 */ J3DModel* mpModel;
    /* 0x5ac */ dBgS_ObjAcch mAcch;
    /* 0x784 */ dBgS_AcchCir mAcchCir;
    /* 0x7c4 */ dCcD_Stts mStts;
    /* 0x7e4 */ dCcD_Cyl mCyl;
    /* 0x904 */ u8 field_0x804[0x944 - 0x93c];
public:
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create();
    int Execute(Mtx**);
    int Draw();
    int Delete();
};

STATIC_ASSERT(sizeof(daObjMBox_c) == 0x944);


#endif /* D_A_OBJ_METALBOX_H */
