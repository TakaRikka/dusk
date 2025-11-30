#ifndef D_A_OBJ_TGAKE_H
#define D_A_OBJ_TGAKE_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"

/**
 * @ingroup actors-objects
 * @class daObjGake_c
 * @brief Howling Cliff
 *
 * @details
 *
 */
class daObjGake_c : public dBgS_MoveBgActor {
public:
    void initBaseMtx();
    void setBaseMtx();
    int create1st();

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    u16 getEventBit1() { return fopAcM_GetParamBit(this, 0, 10); }
    u16 getEventBit2() { return fopAcM_GetParamBit(this, 10, 10); }
    u8 getType() { return fopAcM_GetParamBit(this, 0x14, 8); }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ u16 mEventBit1;
    /* 0x5AE */ u16 mEventBit2;
    /* 0x5B0 */ u8 mHide;
};

STATIC_ASSERT(sizeof(daObjGake_c) == 0x5B4);

#endif /* D_A_OBJ_TGAKE_H */
