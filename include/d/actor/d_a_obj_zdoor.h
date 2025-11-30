#ifndef D_A_OBJ_ZDOOR_H
#define D_A_OBJ_ZDOOR_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daZdoor_c
 * @brief Zelda Door
 *
 * @details
 *
 */
class daZdoor_c : public dBgS_MoveBgActor {
public:
    void init_cyl();
    void set_cyl();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create1st();
    int Execute(Mtx**);
    int Draw();
    int Delete();

    u32 getType() { return fopAcM_GetParamBit(this, 0, 4); }

    /* 0x5A0 */ request_of_phase_process_class mPhaseReq;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ dBgS_ObjAcch mObjAcch;
    /* 0x784 */ dBgS_AcchCir mAcchCir;
    /* 0x7C4 */ dCcD_Stts mStts;
    /* 0x800 */ dCcD_Cyl mCallbackCylinders[4];
    /* 0xCF0 */ dCcD_Cyl mCylinder;
    /* 0xE2C */ s16 mOpenSpeed;
    /* 0xE2E */ s16 mOpenAngle;
    /* 0xE30 */ u8 mDoorType;
};

STATIC_ASSERT(sizeof(daZdoor_c) == 0xE34);


#endif /* D_A_OBJ_ZDOOR_H */
