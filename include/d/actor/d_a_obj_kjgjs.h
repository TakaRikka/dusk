#ifndef D_A_OBJ_KJGJS_H
#define D_A_OBJ_KJGJS_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-objects
 * @class daObjKJgjs_c
 * @brief ???
 *
 * @details
 *
 */
class daObjKJgjs_c : public dBgS_MoveBgActor, public request_of_phase_process_class {
public:
    int create1st();
    void setMtx();
    int CreateHeap();
    int Create();
    int Execute(Mtx**);
    int Draw();
    int Delete();
    virtual ~daObjKJgjs_c();

    u32 getType() { return fopAcM_GetParamBit(this, 0, 4); }

private:
    /* 0x5A8 */ Mtx mMtx[2];
    /* 0x608 */ J3DModel* mpModel;
    /* 0x60C */ s32 field_0x60c;
};

STATIC_ASSERT(sizeof(daObjKJgjs_c) == 0x610);

#endif /* D_A_OBJ_KJGJS_H */
