#ifndef D_A_OBJ_CBOARD_H
#define D_A_OBJ_CBOARD_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-objects
 * @class daObjBoard_c
 * @brief Clear Board
 *
 * @details Actor used to function as an invisible wall.
 *
 */
class daObjBoard_c : public dBgS_MoveBgActor {
public:
    void initBaseMtx();
    void setBaseMtx();
    int create1st();

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    u8 getNameArg() { return fopAcM_GetParamBit(this, 0, 4); }
    u8 getArg0() { return fopAcM_GetParamBit(this, 4, 4); }
    u8 getSwNo() { return fopAcM_GetParamBit(this, 8, 8); }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ Mtx field_0x5a8;
};

STATIC_ASSERT(sizeof(daObjBoard_c) == 0x5D8);

#endif /* D_A_OBJ_CBOARD_H */
