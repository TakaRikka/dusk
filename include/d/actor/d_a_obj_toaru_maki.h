#ifndef D_A_OBJ_TOARU_MAKI_H
#define D_A_OBJ_TOARU_MAKI_H

#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjToaruMaki_c
 * @brief Ordon Bundle
 *
 * @details
 *
 */
class daObjToaruMaki_c : public fopAc_ac_c, public request_of_phase_process_class {
public:
    daObjToaruMaki_c();
    virtual ~daObjToaruMaki_c();
    int createHeap();
    int create();
    int Delete();
    void setModelMtx();
    int draw();
    int execute();

    u32 getNameArg_0() { return fopAcM_GetParamBit(this, 0, 4); }

private:
    /* 0x574 */ J3DModel* mpModel;
    /* 0x578 */ dBgW* mpBgW;
    /* 0x57C */ Mtx mMtx;
};

STATIC_ASSERT(sizeof(daObjToaruMaki_c) == 0x5AC);

#endif /* D_A_OBJ_TOARU_MAKI_H */
